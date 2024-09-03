#ifndef PING_ERRORS_H
#define PING_ERRORS_H

#define ERR_CANNOT_RESOLVE_HOSTNAME "ping: cannot resolve %s: Unknown host\n"
#define ERR_UNKNOWN_HOST "ping: unknown host %s\n"
#define ERR_TEMPORARY_NAME_RESOLUTION "ping: %s: Temporary failure in name resolution\n"

#define ERR_NO_ROUTE_TO_HOST "ping: %s: No route to host\n"

#define ERR_SOCKET_NOT_PERMITTED "ping: socket: Operation not permitted\n"
#define ERR_PERMISSION_DENIED "ping: %s: Permission denied (are you root?)\n"

#define ERR_INVALID_IP_ADDRESS "ping: %s: Name or service not known\n"

#define ERR_NETWORK_UNREACHABLE "ping: connect: Network is unreachable\n"
#define ERR_SENDTO_NETWORK_UNREACHABLE "ping: sendto: Network is unreachable\n"

#define ERR_REQUEST_TIMEOUT "Request timeout for icmp_seq=%d\n"
#define ERR_NO_ANSWER_FROM_HOST "ping: no answer from %s\n"

#define ERR_CANNOT_BIND_SOURCE_ADDRESS "ping: cannot bind to %s: Cannot assign requested address\n"

#define ERR_INVALID_OPTION "ping: invalid option -- '%c'\nUsage: ping [-aAbBdDfhLnOqrRUvV64] [-c count] [-i interval] [-I interface]\n               [-m mark] [-M pmtudisc_option] [-l preload] [-p pattern] [-Q tos]\n               [-s packetsize] [-S sndbuf] [-t ttl] [-T timestamp_option]\n               [-w deadline] [-W timeout] [hop1 ...] destination\n"
#define ERR_INVALID_TOS "ping: invalid TOS: `%s`\n"

#define ERR_PACKET_SIZE_TOO_LARGE "ping: packet size too large: %d > maximum allowed size: %d\n"

#define ERR_NO_SUCH_DEVICE "ping: %s: No such device\n"

#define ERR_ADDRESS_FAMILY_NOT_SUPPORTED "ping: socket: Address family not supported by protocol\n"
#define ERR_PROTOCOL_NOT_SUPPORTED "ping: socket: Protocol not supported\n"

#define ERR_TTL_OUT_OF_RANGE "ping: ttl %d out of range\n"
#define ERR_TTL_EXCEEDED "ping: time to live exceeded\n"

#define ERR_CANNOT_CREATE_SOCKET "ping: cannot create socket: %s\n"

#define ERR_BAD_CHECKSUM "Error: Invalid ICMP packet received (bad checksum).\n"

#define ERR_REGEX_COMPILATION "Error: Could not compile regex for hostname validation.\n"

#define ERR_INVALID_TOS "Ping: invalid TOS: `%c`\n"
#define ERR_INVALID_ARGUMENT "Error: Invalid argument `%s` after destination.\n"
#define ERR_NO_DESTINATION "Error: No destination provided.\n"
#define ERR_INVALID_DESTINATION "Error: Invalid destination '%s'. Must be a valid IPv4 address or a FQDN.\n"

#endif
