#include "kvmpro.h"


const char* Kvmpro::procstat_files(struct procstat *procstat, struct kinfo_proc *kipp) {
    stringstream out;
    struct sockstat sock;
    struct filestat_list *head;
    struct filestat *fst;

    head = procstat_getfiles(procstat, kipp, 0);
    if (head == NULL) {
        fst = NULL;
        kipp = NULL;
        return "{\"status\": \"Failure on: procstat_getfiles(…)\"}";
    }

    STAILQ_FOREACH(fst, head, next) {
        if (fst->fs_type == PS_FST_TYPE_SOCKET) { // only sockets
            if (procstat_get_socket_info(procstat, fst, &sock, NULL) != 0) {
                procstat_freefiles(procstat, head);
                head = NULL;
                fst = NULL;
                kipp = NULL;
                return "{\"status\": \"Failure on: procstat_get_socket_info(…)\"}";
            }
            // Write protocol and data:
            out << " "
                << protocol_to_string(sock.dom_family, sock.type, sock.proto)
                << "="
                << addr_to_string(&sock.sa_local)
                << "->"
                << addr_to_string(&sock.sa_peer);
        }
    }
    procstat_freefiles(procstat, head);
    head = NULL;
    fst = NULL;
    kipp = NULL;

    return out.str().c_str();
}
