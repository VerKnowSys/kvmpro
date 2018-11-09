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
            if ((sock.dom_family == AF_INET) || (sock.dom_family == AF_INET6)) { // only INET domain
                out << protocol_to_string(sock.dom_family, sock.type, sock.proto) << " ";
                if (sock.dom_family == AF_LOCAL) {
                    struct sockaddr_un *sun = (struct sockaddr_un *)&sock.sa_local;
                    if (sun->sun_path[0] != 0)
                        out << addr_to_string(&sock.sa_local);
                    else
                        out << addr_to_string(&sock.sa_peer);
                } else {
                    out << addr_to_string(&sock.sa_local) << "-" << addr_to_string(&sock.sa_peer);
                }
                out << "#";
            }
        }
    }

    procstat_freefiles(procstat, head);
    head = NULL;
    fst = NULL;
    kipp = NULL;

    return out.str().c_str();
}
