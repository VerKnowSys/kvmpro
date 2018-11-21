#include "kvmpro.h"


const char* get_process_usage(int uid) {
    string command, output;
    int count = 0;
    char** args = NULL;
    int pagesize = getpagesize();

    kvm_t* kd = kvm_open(NULL, NULL, NULL, O_RDONLY, NULL);
    if (kd == NULL) {
        args = NULL;
        kd = NULL;
        return (char*)"{\"status\": \"Failure opening kernel KVM handle!\"}";
    }

    kinfo_proc* procs = kvm_getprocs(kd, KERN_PROC_UID, uid, &count); // get processes directly from BSD kernel
    if (count <= 0) {
        procs = NULL;
        kvm_close(kd);
        args = NULL;
        kd = NULL;
        return (char*)"{\"status\": \"No processes for given UID!\"}";
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
        struct kinfo_proc* kproc = procstat_getprocs(procstat, KERN_PROC_PID, procs->ki_pid, &cnt);
        string statinfo = "";
        if (cnt != 0)
            statinfo = procstat_files(procstat, kproc);
        procstat_freeprocs(procstat, kproc);
        procstat_close(procstat);
        procstat = NULL;
        kproc = NULL;

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
            << "\"blk-in\":" << (procs->ki_rusage.ru_inblock) << ","
            << "\"blk-out\":" << (procs->ki_rusage.ru_oublock) << ","
            << "\"thr\":" << (procs->ki_numthreads) << ","
            << "\"pri-nrml\":" << ord(procs->ki_pri.pri_level) << ","
            << "\"statinfo\":\"" << escape_json(statinfo) << "\"}";
        if (i == count - 1) {
            out << "]";
        } else {
            out << ",";
        }
        args = NULL;
        output += out.str();
        procs++;
    }
    kvm_close(kd);
    procs = NULL;
    kd = NULL;
    return output.c_str();
}


const char* get_process_usage_short(int uid) {
    string output;
    int count = 0;
    char** args = NULL;
    const int pagesize = getpagesize();
    const int totalMem = pagesize * sysconf(_SC_PHYS_PAGES);
    /*
        NOTE: from header the prototype is:
        kvm_t* kvm_open(const char *execfile, const char *corefile, const char *swapfile, int flags, const char *errstr);
    */
    kvm_t* kd = kvm_open(NULL, NULL, NULL, O_RDONLY, NULL);
    if (kd == NULL) {
        args = NULL;
        return (char*)"{\"status\": \"Failure on kvm_open(…)\"}";
    }

    kinfo_proc* procs = kvm_getprocs(kd, KERN_PROC_UID, uid, &count); // get processes directly from BSD kernel
    if (count < 0) {
        procs = NULL;
        kvm_close(kd);
        args = NULL;
        kd = NULL;
        return (char*)"{\"status\": \"Failure on: kvm_getprocs(…)\" }";
    }

    output += "{\"status\": \"Process list ready.\", \"list\": [";
    for (int i = 0; i < count; ++i) {
        stringstream out;
        args = kvm_getargv(kd, procs, 0);
        out << "{\"cmd\":\"" << (procs->ki_comm) << "\","
            << "\"pid\":" << (procs->ki_pid) << ","
            << "\"ppid\":" << (procs->ki_ppid) << ","
            << "\"runt\":" << (procs->ki_runtime / 1000) << ","
            << "\"ioin\":" << (procs->ki_rusage.ru_inblock) << ","
            << "\"ioout\":" << (procs->ki_rusage.ru_oublock) << ","
            << "\"rss\":" << (procs->ki_rssize * pagesize) << "}";
        if (i + 1 != count) out << ","; // if last element not detected add a comma
        args = NULL;
        output += out.str();
        procs++;
    }

    kvm_close(kd);
    procs = NULL;
    kd = NULL;
    args = NULL;
    output += "]}";
    return output.c_str();
}
