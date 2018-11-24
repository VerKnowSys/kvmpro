#include "kvmpro.h"


const char* procstat_files(struct procstat *procstat, struct kinfo_proc *kipp) {
    stringstream out;
    struct sockstat sock;
    struct filestat_list *head;
    struct filestat *fst;

    head = procstat_getfiles(procstat, kipp, 0);
    if (head == NULL) {
        return "{\"status\": \"Failure of: procstat_files()\"}";
    }

    STAILQ_FOREACH(fst, head, next) {
        if (fst->fs_type == PS_FST_TYPE_SOCKET) {
            if (procstat_get_socket_info(procstat, fst, &sock, NULL) != 0) {
                procstat_freefiles(procstat, head);
                return "{\"status\": \"Failure of: procstat_get_socket_info()\"}";
            }
            // Write protocol and process details:
            if (out.str().length() > 0)
                out << " ";
            out << protocol_to_string(sock.dom_family, sock.type, sock.proto)
                << "="
                << addr_to_string(&sock.sa_local)
                << "->"
                << addr_to_string(&sock.sa_peer);
        }
    }
    procstat_freefiles(procstat, head);
    return out.str().c_str();
}
