/*
    Author: Daniel (dmilith) Dettlaff
    © 2011-2018 - VerKnowSys
*/


#ifndef __CORE__
#define __CORE__


#define KVMPRO_VERSION "0.2.0"

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
    #include <sys/capability.h>
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


using namespace std;


/* Public functions */


#define EXPORT_SHARED_OBJECT __attribute__ ((visibility("default")))

class Kvmpro {

    private:
        static const string escape_json(const string& input);
        static const char* addr_to_string(struct sockaddr_storage *ss);
        static const char* protocol_to_string(int domain, int type, int protocol);
        static const char* procstat_files(struct procstat *procstat, struct kinfo_proc *kipp); // NOTE: Very FreeBSD specific:

    public:
        EXPORT_SHARED_OBJECT static const char* get_process_usage(int uid);
        EXPORT_SHARED_OBJECT static const char* get_process_usage_short(int uid);

};

#endif
