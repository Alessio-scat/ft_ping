#ifndef FT_PING_H
#define FT_PING_H

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <regex.h>
#include <netdb.h>
#include "ft_ping_error.h"
#include <errno.h>
#include <signal.h>

// Struct to stat PING 
typedef struct {
    int packets_sent;
    int packets_received;
    double rtt_min;
    double rtt_max;
    double rtt_total;
    double rtt_sum_of_squares;
    double total_time;
} ping_stats_t;

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
int receive_icmp_echo_reply(int sockfd, void *recv_icmp_hdr, struct sockaddr_in *src_addr, uint8_t *ttl, 
                             #ifdef __APPLE__
                               struct ip *ip_hdr_copy
                               #else
                               struct iphdr *ip_hdr_copy
                               #endif
                               );

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
void handle_icmp_error_verbose(int activate_v, void *recv_icmp_hdr, struct sockaddr_in *src_addr, int sequence, int *v,
                                #ifdef __APPLE__
                               struct ip *ip_hdr
                               #else
                               struct iphdr *ip_hdr
                               #endif
                               );


/*
    checl_root.c
*/
void check_root_privileges();

/*
    rtt.c
*/

double calculate_and_display_rtt(struct timeval *start, struct timeval *end);

#endif
