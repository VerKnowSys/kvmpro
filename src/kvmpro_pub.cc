#include "kvmpro.hpp"


const string
Kvmpro::get_process_usage(uid_t uid) {
    kvm_t* kd = kvm_open(NULL, NULL, NULL, O_RDONLY, NULL);
    if (kd == NULL) {
        return string("{\"status\": \"Failure on: kvm_open()!\"}");
    }

    int proc_count = 0;
    kinfo_proc* procs = kvm_getprocs(kd, KERN_PROC_UID, uid, &proc_count); // get processes directly from BSD kernel
    if (proc_count <= 0) {
        if (kd)
            kvm_close(kd);
        return string("{\"status\": \"Failure on: kvm_getprocs()!\"}");
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
            return string("{\"status\": \"Failure opening procstat_open_sysctl()!\"}");
        }
        string statinfo = string();
        struct kinfo_proc* kproc = procstat_getprocs(procstat, KERN_PROC_PID, procs->ki_pid, &cnt);
        if (kproc != NULL && cnt > 0) {
            statinfo = Kvmpro::escape_json(Kvmpro::procstat_files(procstat, kproc));
            procstat_freeprocs(procstat, kproc);
            procstat_close(procstat);
        }
        output << "{\"pid\":" << (procs->ki_pid) << ","
            << "\"ppid\":" << (procs->ki_ppid) << ","
            << "\"name\":\"" << Kvmpro::escape_json(procs->ki_comm) << "\","
            << "\"cmd\":\"" << Kvmpro::escape_json(command.str()) << "\","
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
    return output.str();
}


const string
Kvmpro::get_process_usage_short(uid_t uid) {
    kvm_t* kd = kvm_open(NULL, NULL, NULL, O_RDONLY, NULL);
    if (kd == NULL)
        return string("{\"status\": \"Failure on: kvm_open()\"}");

    int proc_count = 0;
    kinfo_proc* procs = kvm_getprocs(kd, KERN_PROC_UID, uid, &proc_count); // get processes directly from BSD kernel
    if ((procs == NULL) || (proc_count < 0)) {
        if (kd)
            kvm_close(kd);
        return string("{\"status\": \"Failure on: kvm_getprocs()\"}");
    }

    stringstream output;
    output << "{\"status\": \"Process list ready.\", \"list\": [";
    for (int i = 0; i < proc_count; ++i) {
        kvm_getargv(kd, procs, 0);
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
    return output.str();
}


/* For easier operability with C symbol names: */
extern "C" {


    EXPORT_SHARED_OBJECT
    const char* get_process_usage(uid_t uid) {
        auto kvmpro_out = Kvmpro::get_process_usage(uid);
        auto my_str_ptr = unique_ptr<string>{new string(kvmpro_out)};
        return my_str_ptr.get()->data();
    }


    EXPORT_SHARED_OBJECT
    const char* get_process_usage_short(uid_t uid) {
        auto kvmpro_out = Kvmpro::get_process_usage_short(uid);
        auto my_str_ptr = unique_ptr<string>{new string(kvmpro_out)};
        return my_str_ptr.get()->data();
    }


    EXPORT_SHARED_OBJECT
    const kvmpro_t get_process_usage_t(uid_t uid) {
        auto kvmpro_out = Kvmpro::get_process_usage(uid);
        kvmpro_t data;
        data.length = kvmpro_out.copy(data.bytes, kvmpro_out.length(), 0);
        return data;
    }


    EXPORT_SHARED_OBJECT
    const kvmpro_t get_process_usage_short_t(uid_t uid) {
        auto kvmpro_out = Kvmpro::get_process_usage_short(uid);
        kvmpro_t data;
        auto length = kvmpro_out.length();
        data.length = kvmpro_out.copy(data.bytes, length, 0);
        return data;
    }


    EXPORT_SHARED_OBJECT
    kvmpro_t* get_process_usage_tp(uid_t uid) {
        auto kvmpro_out = Kvmpro::get_process_usage(uid);
        kvmpro_t* data = (kvmpro_t*)malloc(sizeof(kvmpro_t));
        data->length = kvmpro_out.copy(data->bytes, kvmpro_out.length(), 0);
        return data;
    }


    EXPORT_SHARED_OBJECT
    kvmpro_t* get_process_usage_short_tp(uid_t uid) {
        auto kvmpro_out = Kvmpro::get_process_usage_short(uid);
        kvmpro_t* data = (kvmpro_t*)malloc(sizeof(kvmpro_t));
        data->length = kvmpro_out.copy(data->bytes, kvmpro_out.length(), 0);
        return data;
    }


    EXPORT_SHARED_OBJECT
    void destroy_kvmpro_tp(kvmpro_t* obj) {
        if (obj)
            free(obj);

        obj = nullptr;
    }


}

