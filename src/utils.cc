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
            return ("LOC");
    }
}


const char* Kvmpro::addr_to_string(struct sockaddr_storage *ss) {
    char str[128];
    char portstr[7];
    char buffer2[INET6_ADDRSTRLEN];
    struct sockaddr_in6 *sin6;
    struct sockaddr_in *sin;
    struct sockaddr_un *sun;
    struct sockaddr_dl *sdl;
    stringstream out;
    switch (ss->ss_family) {
        case 255: {
            int addr_no = 0;
            sin = (struct sockaddr_in *)ss;
            while(sin->sin_addr.s_addr & (0x800000 >> addr_no)) addr_no++;
            sprintf(str,"/%d", addr_no);
            out << str;
            break;
        }

        case AF_LOCAL: {
            sun = (struct sockaddr_un *)ss;
            if (sun->sun_path[0] == 0)
                out << "/No/Path";
            else
                out << sun->sun_path;
            break;
        }

        case AF_UNSPEC:
        case AF_INET: {
            sin = (struct sockaddr_in *)ss;
            if (ntohs(sin->sin_port) != 0) {
                snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
            } else {
                snprintf(portstr, sizeof(portstr), ":%s", "0");
            }
            if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) != NULL) {
                out << str << portstr;
            } else {
                out << "0.0.0.0:0";
            }
            break;
        }

        case AF_INET6: {
            sin6 = (struct sockaddr_in6 *)ss;
            if (ntohs(sin->sin_port) != 0) {
                snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
            } else {
                snprintf(portstr, sizeof(portstr), ":%s", "0");
            }
            if (inet_ntop(AF_INET6, &sin6->sin6_addr, buffer2, sizeof(buffer2)) != NULL)
                out << "[" << buffer2 << "]" << portstr;
            else
                out << "[::1]" << portstr;
            break;
        }

        case AF_LINK: {
            sdl = (struct sockaddr_dl *)ss;
            if (sdl->sdl_nlen > 0) {
                snprintf(str, sizeof(str), "%*s", sdl->sdl_nlen, &sdl->sdl_data[0]);
            } else {
                snprintf(str, sizeof(str), "link#%d", sdl->sdl_index);
            }
            out << str;
            break;
        }

        default:
            out << "*:*";
            break;
    }
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

