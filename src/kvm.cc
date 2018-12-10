#include "kvmpro.h"


const char* get_process_usage(uid_t uid) {
    string command, output;
    int count = 0;
    char** args = NULL;
    int pagesize = getpagesize();

    kvm_t* kd = kvm_open(NULL, NULL, NULL, O_RDONLY, NULL);
    if (kd == NULL) {
        return (const char*)"{\"status\": \"Failure opening kernel KVM handle!\"}";
    }

    kinfo_proc* procs = kvm_getprocs(kd, KERN_PROC_UID, uid, &count); // get processes directly from BSD kernel
    if (count <= 0) {
        if (kd)
            kvm_close(kd);
        return (const char*)"{\"status\": \"No processes for given UID!\"}";
    }

    for (int i = 0; i < count; ++i) {
        stringstream out;
        command = "";
        args = kvm_getargv(kd, procs, 0);
        for (int y = 0; (args != 0) && (args[y] != 0); y++)
            if (y == 0)
                command = string(args[y]);
            else
                command += " " + string(args[y]);

        unsigned int cnt = 0;
        struct procstat* procstat = procstat_open_sysctl();
        if (procstat == NULL) {
            if (kd)
                kvm_close(kd);
            return (const char*)"{\"status\": \"Failure opening procstat_open_sysctl()!\", \"list\": []}";
        }
        struct kinfo_proc* kproc = procstat_getprocs(procstat, KERN_PROC_PID, procs->ki_pid, &cnt);
        string statinfo = "";
        if (cnt != 0)
            statinfo = procstat_files(procstat, kproc);

        // Render JSON directly:
        if (i == 0) {
            out << "[";
        }
        out << "{\"pid\":" << (procs->ki_pid) << ","
            << "\"ppid\":" << (procs->ki_ppid) << ","
            << "\"name\":\"" << escape_json(procs->ki_comm) << "\","
            << "\"cmd\":\"" << escape_json(command) << "\","
            << "\"rss\":" << (procs->ki_rssize * pagesize) << ","
            << "\"mrss\":" << (procs->ki_rusage.ru_maxrss * pagesize) << ","
            << "\"runtime\":" << (procs->ki_runtime / 1000) << ","
            << "\"blk_in\":" << (procs->ki_rusage.ru_inblock) << ","
            << "\"blk_out\":" << (procs->ki_rusage.ru_oublock) << ","
            << "\"nthr\":" << (procs->ki_numthreads) << ","
            << "\"pri_level\":" << ord(procs->ki_pri.pri_level) << ","
            << "\"stat_info\":\"" << escape_json(statinfo) << "\"}";
        if (i == count - 1) {
            out << "]";
        } else {
            out << ",";
        }
        output += out.str();
        procs++;
    }
    if (kd)
        kvm_close(kd);
    return (const char*)output.data();
}


const char* get_process_usage_short(uid_t uid) {
    string output;
    int count = 0;
    char** args = NULL;
    /*
        NOTE: from header the prototype is:
        kvm_t* kvm_open(const char *execfile, const char *corefile, const char *swapfile, int flags, const char *errstr);
    */
    kvm_t* kd = kvm_open(NULL, NULL, NULL, O_RDONLY, NULL);
    if (kd == NULL) {
        return (const char*)"{\"status\": \"Failure on kvm_open(…)\"}";
    }

    kinfo_proc* procs = kvm_getprocs(kd, KERN_PROC_UID, uid, &count); // get processes directly from BSD kernel
    if (count < 0) {
        if (kd)
            kvm_close(kd);
        return (const char*)"{\"status\": \"Failure on: kvm_getprocs(…)\" }";
    }

    output += "{\"status\": \"Process list ready.\", \"list\": [";
    for (int i = 0; i < count; ++i) {
        stringstream out;
        args = kvm_getargv(kd, procs, 0);
        out << "{\"cmd\":\"" << (procs->ki_comm) << "\","
            << "\"pid\":" << (procs->ki_pid) << ","
            << "\"ppid\":" << (procs->ki_ppid) << ","
            << "\"runtime\":" << (procs->ki_runtime / 1000) << ","
            << "\"blk_in\":" << (procs->ki_rusage.ru_inblock) << ","
            << "\"blk_out\":" << (procs->ki_rusage.ru_oublock) << ","
            << "\"rss\":" << (procs->ki_rssize * getpagesize()) << "}";
        if (i + 1 != count) out << ","; // if last element not detected add a comma
        output += out.str();
        procs++;
    }
    if (kd)
        kvm_close(kd);
    output += "]}";
    return (const char*)output.data();
}
