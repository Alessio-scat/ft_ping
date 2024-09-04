#include "../include/ft_ping.h"

/*
    RFC 792 : Internet Control Message Protocole
    https://datatracker.ietf.org/doc/html/rfc792
*/

void handle_icmp_error_verbose(struct icmphdr *recv_icmp_hdr, struct sockaddr_in *src_addr, int sequence, int *v) {
    // Check if it's not an ICMP ECHO_REPLY packet
    if (recv_icmp_hdr->type != ICMP_ECHOREPLY) {
        *v = 1;
        switch (recv_icmp_hdr->type) {
            case 3:  // Destination Unreachable
                switch (recv_icmp_hdr->code) {
                    case 0:
                        printf("From %s: icmp_seq=%d Destination Network Unreachable\n", inet_ntoa(src_addr->sin_addr), sequence);
                        break;
                    case 1:
                        printf("From %s: icmp_seq=%d Destination Host Unreachable\n", inet_ntoa(src_addr->sin_addr), sequence);
                        break;
                    case 2:
                        printf("From %s: icmp_seq=%d Destination Protocol Unreachable\n", inet_ntoa(src_addr->sin_addr), sequence);
                        break;
                    case 3:
                        printf("From %s: icmp_seq=%d Destination Port Unreachable\n", inet_ntoa(src_addr->sin_addr), sequence);
                        break;
                    case 13:
                        printf("From %s: icmp_seq=%d Communication administratively prohibited\n", inet_ntoa(src_addr->sin_addr), sequence);
                        break;
                    default:
                        printf("From %s: icmp_seq=%d Destination Unreachable, Bad code: %d\n", inet_ntoa(src_addr->sin_addr), sequence, recv_icmp_hdr->code);
                        break;
                }
                break;

            case 11:  // Time Exceeded
                switch (recv_icmp_hdr->code) {
                    case 0:
                        printf("From %s: icmp_seq=%d Time to live exceeded\n", inet_ntoa(src_addr->sin_addr), sequence);
                        break;
                    case 1:
                        printf("From %s: icmp_seq=%d Fragment reassembly time exceeded\n", inet_ntoa(src_addr->sin_addr), sequence);
                        break;
                    default:
                        printf("From %s: icmp_seq=%d Time Exceeded, Bad code: %d\n", inet_ntoa(src_addr->sin_addr), sequence, recv_icmp_hdr->code);
                        break;
                }
                break;

            case 12:
                switch(recv_icmp_hdr->code){
                    case 0:
                        printf("From %s: icmp_seq=%d pointer indicates the error\n", inet_ntoa(src_addr->sin_addr), sequence);
                        break;
                }

            case 4:
                switch(recv_icmp_hdr->code){
                    case 0:
                        printf("From %s: icmp_seq=%d Source Quench\n", inet_ntoa(src_addr->sin_addr), sequence);
                        break;
                }

            case 5:  // Redirect
                switch (recv_icmp_hdr->code) {
                    case 0:
                        printf("From %s: icmp_seq=%d Redirect Network\n", inet_ntoa(src_addr->sin_addr), sequence);
                        break;
                    case 1:
                        printf("From %s: icmp_seq=%d Redirect Host\n", inet_ntoa(src_addr->sin_addr), sequence);
                        break;
                    case 2:
                        printf("From %s: icmp_seq=%d Redirect Network for Type of Service\n", inet_ntoa(src_addr->sin_addr), sequence);
                        break;
                    case 3:
                        printf("From %s: icmp_seq=%d Redirect Host for Type of Service\n", inet_ntoa(src_addr->sin_addr), sequence);
                        break;
                    default:
                        printf("From %s: icmp_seq=%d Redirect, Bad code: %d\n", inet_ntoa(src_addr->sin_addr), sequence, recv_icmp_hdr->code);
                        break;
                }
                break;

            case 13:  // Timestamp Request
                printf("From %s: icmp_seq=%d Timestamp Request\n", inet_ntoa(src_addr->sin_addr), sequence);
                break;

            case 14:  // Timestamp Reply
                printf("From %s: icmp_seq=%d Timestamp Reply\n", inet_ntoa(src_addr->sin_addr), sequence);
                break;

            case 15:  // Information Request
                printf("From %s: icmp_seq=%d Information Request\n", inet_ntoa(src_addr->sin_addr), sequence);
                break;

            case 16:  // Information Reply
                printf("From %s: icmp_seq=%d Information Reply\n", inet_ntoa(src_addr->sin_addr), sequence);
                break;

            default:
                printf("From %s: icmp_seq=%d Unknown ICMP type: %d code: %d\n", inet_ntoa(src_addr->sin_addr), sequence, recv_icmp_hdr->type, recv_icmp_hdr->code);
                break;
        }
    }
}
