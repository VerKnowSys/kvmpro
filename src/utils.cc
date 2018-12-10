#include "kvmpro.h"


const char* protocol_to_string(int domain, int type, int protocol) {
    switch (domain) {
        case AF_INET:
        case AF_INET6:
            switch (protocol) {
                case IPPROTO_TCP:
                    return (const char*)"TCP";
                case IPPROTO_UDP:
                    return (const char*)"UDP";
                case IPPROTO_ICMP:
                    return (const char*)"ICM";
                case IPPROTO_RAW:
                    return (const char*)"RAW";
                case IPPROTO_SCTP:
                    return (const char*)"SCT";
                case IPPROTO_DIVERT:
                    return (const char*)"IPD";
                default:
                    return (const char*)"IP?";
            }
        case AF_LOCAL:
            switch (type) {
                case SOCK_STREAM:
                    return (const char*)"UDS";
                case SOCK_DGRAM:
                    return (const char*)"UDD";
                default:
                    return (const char*)"UD?";
            }
        default:
            return (const char*)"LOC";
    }
}


const char* addr_to_string(struct sockaddr_storage *ss) {
    char portstr[7];
    char str[512];
    char stripv6[INET6_ADDRSTRLEN];
    struct sockaddr_in6 *sin6;
    struct sockaddr_in *sin;
    struct sockaddr_un *sun;
    struct sockaddr_dl *sdl;
    stringstream out;
    switch (ss->ss_family) {
        // case 255: {
        //     int addr_no = 0;
        //     sin = (struct sockaddr_in *)ss;
        //     while(sin->sin_addr.s_addr & (0x800000 >> addr_no)) addr_no++;
        //     sprintf(str,"/%d", addr_no);
        //     out << str;
        //     break;
        // }

        case AF_LOCAL: {
            sun = (struct sockaddr_un *)ss;
            if (sun->sun_path[0] == 0)
                out << "*";
            else
                out << sun->sun_path;
            break;
        }

        case AF_INET: {
            sin = (struct sockaddr_in *)ss;
            if (ntohs(sin->sin_port) != 0) {
                snprintf(portstr, sizeof(portstr), "%d", ntohs(sin->sin_port));
            } else {
                snprintf(portstr, sizeof(portstr), "%d", 0);
            }
            if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) != NULL) {
                out << str << ":" << portstr;
            } else {
                out << "0.0.0.0" << ":" << portstr;
            }
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

        case AF_INET6: {
            sin6 = (struct sockaddr_in6 *)ss;
            if (ntohs(sin6->sin6_port) != 0) {
                snprintf(portstr, sizeof(portstr), "%d", ntohs(sin6->sin6_port));
            } else {
                snprintf(portstr, sizeof(portstr), "%d", 0);
            }
            if (inet_ntop(AF_INET6, &sin6->sin6_addr, stripv6, sizeof(stripv6)) != NULL)
                out << "[" << stripv6 << "]" << ":" << portstr;
            else
                out << "[::1]" << ":" << portstr;
            break;
        }

        case AF_UNSPEC:
        default:
            out << "0.0.0.0" << ":" << "0";
    }
    return (const char*)out.str().data();
}


const string escape_json(const string& input) {
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

