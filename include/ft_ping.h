#ifndef FT_PING_H
#define FT_PING_H

#include <stdint.h>  // To types u_int8_t, u_int16_t, etc...
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
// #ifdef __APPLE__
#include <netinet/ip_icmp.h>
// #else
// #include <netinet/ip_icmp.h>
// #endif
// #include <netinet/ip.h>
// #ifdef __APPLE__
#include <netinet/ip.h>  //IP header
// #else
// #include <netinet/ip.h>
// #endif
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h> // to inet_pton
#include <regex.h>
#include <netdb.h>
#include "ft_ping_error.h"
#include <errno.h>
#include <signal.h>

// #ifdef __APPLE__
// // Define struct icmphdr to macOS
// struct icmphdr {
//     u_int8_t type;
//     u_int8_t code;
//     u_int16_t checksum;
//     union {
//         struct {
//             u_int16_t id;
//             u_int16_t sequence;
//         } echo;
//         u_int32_t gateway;
//         struct {
//             u_int16_t unused_field;
//             u_int16_t mtu;
//         } frag;
//     } un;
// };

// // Define struct iphdr
// struct iphdr {
//     uint8_t ihl:4, version:4;
//     uint8_t tos;
//     uint16_t tot_len;
//     uint16_t id;
//     uint16_t frag_off;
//     uint8_t ttl;
//     uint8_t protocol;
//     uint16_t check;
//     uint32_t saddr;
//     uint32_t daddr;
// };
// #endif

// Struct to stat PING 
typedef struct {
    int packets_sent;
    int packets_received;
    double rtt_min;
    double rtt_max;
    double rtt_total;
    double rtt_sum_of_squares;
} ping_stats_t;

/*
    @extern : les variables sont déclarer ailleurs 
*/
extern volatile int running;
extern ping_stats_t stats;
extern int g_status;

/*
    displayStats.c
*/
void handle_interrupt(int sig);
void calculate_and_display_rtt_statistics(struct timeval *start, struct timeval *end, ping_stats_t *stats);
void calculate_and_display_statistics(ping_stats_t *stats, int tv);

/*
    icmp.c
*/
void construct_icmp_echo_request(void *icmp_hdr, int sequence);
void send_icmp_echo_request(int sockfd, struct sockaddr_in *dest_addr, void *icmp_hdr);
int receive_icmp_echo_reply(int sockfd, void *recv_icmp_hdr, struct sockaddr_in *src_addr, uint8_t *ttl);

/*
    checksum.c
*/
unsigned short checksum(void *b, int len);

/*
    parsing.c
*/
void parse_command_line(int ac, char **av, char **destination, int *verbose);

/*
    verbose.c
*/
void handle_icmp_error_verbose(void *recv_icmp_hdr, struct sockaddr_in *src_addr, int sequence, int *v);


/*
    checl_root.c
*/
void check_root_privileges();

/*
    rtt.c
*/

double calculate_and_display_rtt(struct timeval *start, struct timeval *end);

#endif
