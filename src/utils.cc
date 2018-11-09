#include "kvmpro.h"


const char* Kvmpro::protocol_to_string(int domain, int type, int protocol) {
    switch (domain) {
        case AF_INET:
        case AF_INET6:
            switch (protocol) {
            case IPPROTO_TCP:
                return ("TCP");
            case IPPROTO_UDP:
                return ("UDP");
            case IPPROTO_ICMP:
                return ("ICM");
            case IPPROTO_RAW:
                return ("RAW");
            case IPPROTO_SCTP:
                return ("SCT");
            case IPPROTO_DIVERT:
                return ("IPD");
            default:
                return ("IP?");
            }
        case AF_LOCAL:
            switch (type) {
            case SOCK_STREAM:
                return ("UDS");
            case SOCK_DGRAM:
                return ("UDD");
            default:
                return ("UD?");
            }
        default:
            return ("?");
    }
}


const char* Kvmpro::addr_to_string(struct sockaddr_storage *ss) {
    char buffer2[INET6_ADDRSTRLEN];
    struct sockaddr_in6 *sin6;
    struct sockaddr_in *sin;
    struct sockaddr_un *sun;
    stringstream out;
    switch (ss->ss_family) {
        case AF_LOCAL:
            sun = (struct sockaddr_un *)ss;
            out << sun->sun_path;
            break;
        case AF_INET:
            sin = (struct sockaddr_in *)ss;
            out << inet_ntoa(sin->sin_addr) << ":" << ntohs(sin->sin_port);
            break;
        case AF_INET6:
            sin6 = (struct sockaddr_in6 *)ss;
            if (inet_ntop(AF_INET6, &sin6->sin6_addr, buffer2, sizeof(buffer2)) != NULL)
                out << buffer2 << "." << ntohs(sin6->sin6_port);
            break;
        default:
            out << "*:*";
            break;
    }
    sin6 = NULL;
    sin = NULL;
    sun = NULL;
    return out.str().c_str();
}


const string Kvmpro::escape_json(const string& input) {
    std::ostringstream ss;
    // C++11:
    // for (auto iter = input.cbegin(); iter != input.cend(); iter++) {
    // C++98/03:
    for (std::string::const_iterator iter = input.begin(); iter != input.end(); iter++) {
        switch (*iter) {
            case '\\': ss << "\\\\"; break;
            case '"': ss << "\\\""; break;
            // case '/': ss << "\\/"; break;
            case '\b': ss << "\\b"; break;
            case '\f': ss << "\\f"; break;
            case '\n': ss << "\\n"; break;
            case '\r': ss << "\\r"; break;
            case '\t': ss << "\\t"; break;
            default: ss << *iter; break;
        }
    }
    return ss.str();
}

