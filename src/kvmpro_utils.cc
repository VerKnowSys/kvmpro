#include "kvmpro.h"


const string
protocol_to_string(const int domain, const int type, const int protocol) {
    switch (domain) {
        case AF_INET:
        case AF_INET6:
            switch (protocol) {
                case IPPROTO_TCP:
                    return string("TCP");
                case IPPROTO_UDP:
                    return string("UDP");
                case IPPROTO_ICMP:
                    return string("ICM");
                case IPPROTO_RAW:
                    return string("RAW");
                case IPPROTO_SCTP:
                    return string("SCT");
                case IPPROTO_DIVERT:
                    return string("IPD");
                default:
                    return string("IP?");
            }
        case AF_LOCAL:
            switch (type) {
                case SOCK_STREAM:
                    return string("UDS");
                case SOCK_DGRAM:
                    return string("UDD");
                default:
                    return string("UD?");
            }
        default:
            return string("LOC");
    }
}


const string
addr_to_string(const struct sockaddr_storage *ss) {
    stringstream out;
    char port_str[7];
    char addr_strv4[INET_ADDRSTRLEN];
    char addr_strv6[INET6_ADDRSTRLEN];
    struct sockaddr_in6 *sin6;
    struct sockaddr_in *sin;
    struct sockaddr_un *sun;
    struct sockaddr_dl *sdl;

    switch (ss->ss_family) {
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
            if (ntohs(sin->sin_port) != 0)
                snprintf(port_str, sizeof(port_str), "%d", ntohs(sin->sin_port));
            else
                snprintf(port_str, sizeof(port_str), "%d", 0);

            if (inet_ntop(AF_INET, &sin->sin_addr, addr_strv4, sizeof(addr_strv4)) != NULL) {
                out << addr_strv4 << ":" << port_str;
            } else {
                out << "0.0.0.0/0";
            }
            break;
        }

        case AF_LINK: {
            sdl = (struct sockaddr_dl *)ss;
            if (sdl->sdl_nlen > 0)
                snprintf(addr_strv4, sizeof(addr_strv4), "%*s", sdl->sdl_nlen, &sdl->sdl_data[0]);
            else
                snprintf(addr_strv4, sizeof(addr_strv4), "link#%d", sdl->sdl_index);

            out << addr_strv4;
            break;
        }

        case AF_INET6: {
            sin6 = (struct sockaddr_in6 *)ss;
            if (ntohs(sin6->sin6_port) != 0)
                snprintf(port_str, sizeof(port_str), "%d", ntohs(sin6->sin6_port));
            else
                snprintf(port_str, sizeof(port_str), "%d", 0);

            if (inet_ntop(AF_INET6, &sin6->sin6_addr, addr_strv6, sizeof(addr_strv6)) != NULL)
                out << "[" << addr_strv6 << "]" << ":" << port_str;
            else
                out << "[::1]" << ":" << port_str;
            break;
        }

        case AF_UNSPEC:
        default:
            out << "::/0";
    }
    return out.str();
}


const string
escape_json(const string& input) {
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

