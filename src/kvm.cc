#include "kvmpro.h"


const char* get_process_usage(const uid_t uid) {
    kvm_t* kd = kvm_open(NULL, NULL, NULL, O_RDONLY, NULL);
    if (kd == NULL) {
        return (const char*)"{\"status\": \"Failure on: kvm_open()!\"}";
    }

    int proc_count = 0;
    kinfo_proc* procs = kvm_getprocs(kd, KERN_PROC_UID, uid, &proc_count); // get processes directly from BSD kernel
    if (proc_count <= 0) {
        if (kd)
            kvm_close(kd);
        return (const char*)"{\"status\": \"Failure on: kvm_getprocs()!\"}";
    }

    stringstream output;
    output << "{\"status\": \"Full process list ready.\", \"list\": [";
    for (int i = 0; i < proc_count; ++i) {
        stringstream command;
        char** args = kvm_getargv(kd, procs, 0);
        for (int y = 0; (args != 0) && (args[y] != 0); y++)
            if (y == 0)
                command << string(args[y]);
            else
                command << " " << string(args[y]);

        unsigned int cnt = 0;
        struct procstat* procstat = procstat_open_sysctl();
        if (procstat == NULL) {
            if (kd)
                kvm_close(kd);
            return (const char*)"{\"status\": \"Failure opening procstat_open_sysctl()!\"}";
        }
        string statinfo = string();
        struct kinfo_proc* kproc = procstat_getprocs(procstat, KERN_PROC_PID, procs->ki_pid, &cnt);
        if (kproc != NULL && cnt > 0) {
            statinfo = escape_json(procstat_files(procstat, kproc));
            procstat_freeprocs(procstat, kproc);
            procstat_close(procstat);
        }
        output << "{\"pid\":" << (procs->ki_pid) << ","
            << "\"ppid\":" << (procs->ki_ppid) << ","
            << "\"name\":\"" << escape_json(procs->ki_comm) << "\","
            << "\"cmd\":\"" << escape_json(command.str()) << "\","
            << "\"rss\":" << (procs->ki_rssize * getpagesize()) << ","
            << "\"mrss\":" << (procs->ki_rusage.ru_maxrss * getpagesize()) << ","
            << "\"runtime\":" << (procs->ki_runtime / 1000) << ","
            << "\"blk_in\":" << (procs->ki_rusage.ru_inblock) << ","
            << "\"blk_out\":" << (procs->ki_rusage.ru_oublock) << ","
            << "\"nthr\":" << (procs->ki_numthreads) << ","
            << "\"pri_level\":" << ord(procs->ki_pri.pri_level) << ","
            << "\"stat_info\":\"" << statinfo << "\"}";
        if (i == proc_count - 1)
            output << "]}";
        else
            output << ",";

        procs++;
    }
    if (kd)
        kvm_close(kd);
    return (const char*)output.str().data();
}


const char* get_process_usage_short(const uid_t uid) {
    kvm_t* kd = kvm_open(NULL, NULL, NULL, O_RDONLY, NULL);
    if (kd == NULL)
        return (const char*)"{\"status\": \"Failure on: kvm_open()\"}";

    int proc_count = 0;
    kinfo_proc* procs = kvm_getprocs(kd, KERN_PROC_UID, uid, &proc_count); // get processes directly from BSD kernel
    if ((procs == NULL) || (proc_count < 0)) {
        if (kd)
            kvm_close(kd);
        return (const char*)"{\"status\": \"Failure on: kvm_getprocs()\"}";
    }

    stringstream output;
    output << "{\"status\": \"Process list ready.\", \"list\": [";
    for (int i = 0; i < proc_count; ++i) {
        char** args = kvm_getargv(kd, procs, 0);
        output << "{\"cmd\":\"" << (procs->ki_comm) << "\","
            << "\"pid\":" << (procs->ki_pid) << ","
            << "\"ppid\":" << (procs->ki_ppid) << ","
            << "\"runtime\":" << (procs->ki_runtime / 1000) << ","
            << "\"blk_in\":" << (procs->ki_rusage.ru_inblock) << ","
            << "\"blk_out\":" << (procs->ki_rusage.ru_oublock) << ","
            << "\"rss\":" << (procs->ki_rssize * getpagesize()) << "}";
        if (i + 1 != proc_count)
            output << ","; // if last element not detected add a comma

        procs++;
    }
    output << "]}";
    if (kd)
        kvm_close(kd);
    return (const char*)output.str().data();
}
