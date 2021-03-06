/*
    Author: Daniel (dmilith) Dettlaff
    © 2011-2018 - VerKnowSys
*/


#ifndef __KVM_PRO__
#define __KVM_PRO__


#define KVMPRO_VERSION "0.6.16"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

#include <paths.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/user.h>
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <libgen.h>
#include <getopt.h>
#include <termios.h>
#include <signal.h>
#include <dirent.h>


#ifdef __FreeBSD__
    #include <kvm.h>
    #include <fcntl.h>
    #include <libprocstat.h>
    #include <libutil.h>
    #include <net/if_dl.h>
    #define ord(c) ((int)(unsigned char)(c))
#elif __sun
    #error "Solaris/Illumos is not supported as production platform."
#elif __linux__
    #error "Linux is not supported as production platform."
#elif __osf__
    #error "Tru64 is not supported as production platform."
#elif __sgi
    #error "Irix is not supported as production platform."
#elif _AIX
    #error "AIX is not supported as production platform."
#elif __hpux
    #error "HP-UX is not supported as production platform."
#elif __OpenBSD__
    #error "OpenBSD is not supported as production platform."
#elif __NetBSD__
    #error "NetBSD is not supported as production platform."
#elif __APPLE__
    #error "Darwin is not supported as production platform."
#else
    #error "Unsupported unknown platform."
#endif

#define EXPORT_SHARED_OBJECT __attribute__ ((visibility("default")))


using namespace std;


class Kvmpro {

    // Public Functions provided by this library:
    public:

        // JSON escape helper for C++:
        static const string escape_json(const string& input);

        // convert given protocol to plain string:
        static const string protocol_to_string(const int domain, const int type, const int protocol);

        // convert given address to plain string:
        static const string addr_to_string(const struct sockaddr_storage *ss);

        // read procstat data from kernel:
        static const string procstat_files(struct procstat *procstat, struct kinfo_proc *kipp); // NOTE: FreeBSD/HardenedBSD specific feature:


        // generates JSON of processes which belong to user with given "uid".
        // this list contains every process information available, including open sockets,
        // open network connections, protocols, bound addresses and ports.
        static const string get_process_usage(uid_t uid);

        // generates simplified JSON of processes which belong to user with given "uid".
        // (simplified data structure without "socketstat" field and additional, often rarely used fields).
        static const string get_process_usage_short(uid_t uid);

};




typedef struct {

    size_t length;
    uint8_t bytes[262144]; /* 256 KiB is max for JSON payload */

} kvmpro_t;



/* For much easier operability with C symbol names: */
extern "C" {


    EXPORT_SHARED_OBJECT
    const char* get_process_usage(uid_t uid);


    EXPORT_SHARED_OBJECT
    const char* get_process_usage_short(uid_t uid);


    EXPORT_SHARED_OBJECT
    const kvmpro_t get_process_usage_t(uid_t uid);


    EXPORT_SHARED_OBJECT
    const kvmpro_t get_process_usage_short_t(uid_t uid);


    EXPORT_SHARED_OBJECT
    kvmpro_t* get_process_usage_tp(uid_t uid);


    EXPORT_SHARED_OBJECT
    kvmpro_t* get_process_usage_short_tp(uid_t uid);


    EXPORT_SHARED_OBJECT
    void destroy_kvmpro_tp(kvmpro_t* obj);

}


#endif
