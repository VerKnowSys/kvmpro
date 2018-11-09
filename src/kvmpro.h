/*
    Author: Daniel (dmilith) Dettlaff
    © 2011-2018 - VerKnowSys
*/


#ifndef __CORE__
#define __CORE__


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


#ifdef __APPLE__
    #error "Darwin is not supported as production platform."
#endif

#ifdef __linux__
    #error "Linux is not supported as production platform."
#endif

#ifdef __FreeBSD__
    #include <kvm.h>
    #include <sys/capability.h>
    #include <libprocstat.h>
    #include <libutil.h>
    #include <net/if_dl.h>
    #define ord(c) ((int)(unsigned char)(c))
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
        static const bool invalid_char(char c);
        static const void strip_unicode(string& str);

    public:
        EXPORT_SHARED_OBJECT static const char* get_process_usage(int uid);
        EXPORT_SHARED_OBJECT static const char* get_process_usage_short(int uid);

};

#endif
