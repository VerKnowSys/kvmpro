#include "kvmpro.h"


const string procstat_files(struct procstat *procstat, struct kinfo_proc *kipp) {
    struct sockstat sock;
    struct filestat *fst;
    struct filestat_list *list_head = procstat_getfiles(procstat, kipp, 1);
    if (list_head == NULL) {
        return string("{\"status\": \"Failure of: procstat_getfiles()\"}");
    }

    stringstream output;
    STAILQ_FOREACH(fst, list_head, next) {
        if (fst->fs_type == PS_FST_TYPE_SOCKET) {
            if (procstat_get_socket_info(procstat, fst, &sock, NULL) != 0) {
                if (list_head)
                    procstat_freefiles(procstat, list_head);
                return string("{\"status\": \"Failure of: procstat_get_socket_info()\"}");
            }
            // Write protocol and process details:
            if (!output.str().empty())
                output << " ";
            output << protocol_to_string(sock.dom_family, sock.type, sock.proto)
                << "="
                << addr_to_string(&sock.sa_local)
                << "->"
                << addr_to_string(&sock.sa_peer);
        }
    }
    if (list_head)
        procstat_freefiles(procstat, list_head);
    return output.str();
}
