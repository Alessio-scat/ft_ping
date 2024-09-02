#ifndef FT_PING_H
#define FT_PING_H

#include <stdint.h>  // To types u_int8_t, u_int16_t, etc...

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>  // to ICMP headers
#include <unistd.h>
#include <sys/time.h>

// Define struct icmphdr to macOS
struct icmphdr {
    u_int8_t type;
    u_int8_t code;
    u_int16_t checksum;
    union {
        struct {
            u_int16_t id;
            u_int16_t sequence;
        } echo;
        u_int32_t gateway;
        struct {
            u_int16_t unused_field;
            u_int16_t mtu;
        } frag;
    } un;
};

#endif
