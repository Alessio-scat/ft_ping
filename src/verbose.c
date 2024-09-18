#include "../include/ft_ping.h"

/*
    RFC 792 : Internet Control Message Protocole
    https://datatracker.ietf.org/doc/html/rfc792
*/


#include <netinet/ip.h>
#ifdef __APPLE__
#include <netinet/ip_icmp.h> 
typedef struct icmp icmp_t;
#else
#include <netinet/ip_icmp.h>
typedef struct icmphdr icmp_t;
#endif

void handle_icmp_error_verbose(int activate_v, void *recv_icmp_hdr, struct sockaddr_in *src_addr, int sequence, int *v,
                                #ifdef __APPLE__
                               struct ip *ip_hdr
                               #else
                               struct iphdr *ip_hdr
                               #endif
                               )
{
    int icmp_type;
    int icmp_code;

    #ifdef __APPLE__
        struct icmp *hdr = (struct icmp *)recv_icmp_hdr;
        icmp_type = hdr->icmp_type;
        icmp_code = hdr->icmp_code;
    #else
        struct icmphdr *hdr = (struct icmphdr *)recv_icmp_hdr;
        icmp_type = hdr->type;
        icmp_code = hdr->code;
    #endif

    printf("%d\n", icmp_type);
    (void)activate_v;
    if (icmp_type != ICMP_ECHOREPLY) {
        *v = 1;

        if (activate_v == 1)
        {
            printf("---- IP Header ----\n");
            printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src      Dst Data\n");
            #ifdef __APPLE__
                printf(" %1x  %1x  %02x %04x %04x", ip_hdr->ip_v, ip_hdr->ip_hl, ip_hdr->ip_tos, ip_hdr->ip_len, ip_hdr->ip_id);
                printf("   %1x %04x", ((ip_hdr->ip_off) & 0xe000) >> 13, (ip_hdr->ip_off) & 0x1fff);
                printf("  %02x  %02x %04x", ip_hdr->ip_ttl, ip_hdr->ip_p, ip_hdr->ip_sum);
                printf(" %s ", inet_ntoa(*(struct in_addr *)&ip_hdr->ip_src));
                printf(" %s ", inet_ntoa(*(struct in_addr *)&ip_hdr->ip_dst));
            #else
                printf(" %1x  %1x  %02x %04x %04x", ip_hdr->version, ip_hdr->ihl, ip_hdr->tos, ntohs(ip_hdr->tot_len), ntohs(ip_hdr->id));
                printf("   %1x %04x", ((ip_hdr->frag_off) & 0xe000) >> 13, (ip_hdr->frag_off) & 0x1fff);
                printf("  %02x  %02x %04x", ip_hdr->ttl, ip_hdr->protocol, ip_hdr->check);
                printf(" %s ", inet_ntoa(*(struct in_addr *)&ip_hdr->saddr));
                printf(" %s ", inet_ntoa(*(struct in_addr *)&ip_hdr->daddr));
            #endif
            printf("\n");

            // Afficher les informations d'en-tête ICMP
            printf("---- ICMP Header ----\n");
            printf("Type: %d\n", icmp_type);
            printf("Code: %d\n", icmp_code);
            #ifdef __APPLE__
                printf("Checksum: %04x\n", ntohs(hdr->icmp_cksum));
            #else
                printf("Checksum: %04x\n", ntohs(hdr->checksum));
            #endif
        }

        switch (icmp_type) {
            case 3:  // Destination Unreachable
                switch (icmp_code) {
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
                        printf("From %s: icmp_seq=%d Destination Unreachable, Bad code: %d\n", inet_ntoa(src_addr->sin_addr), sequence, icmp_code);
                        break;
                }
                break;

            case 11:  // Time Exceeded
                switch (icmp_code) {
                    case 0:
                        printf("From %s: icmp_seq=%d Time to live exceeded\n", inet_ntoa(src_addr->sin_addr), sequence);
                        break;
                    case 1:
                        printf("From %s: icmp_seq=%d Fragment reassembly time exceeded\n", inet_ntoa(src_addr->sin_addr), sequence);
                        break;
                    default:
                        printf("From %s: icmp_seq=%d Time Exceeded, Bad code: %d\n", inet_ntoa(src_addr->sin_addr), sequence, icmp_code);
                        break;
                }
                break;

            case 12:  // Pointer Problem
                if (icmp_code == 0) {
                    printf("From %s: icmp_seq=%d Pointer indicates the error\n", inet_ntoa(src_addr->sin_addr), sequence);
                }
                break;

            case 4:  // Source Quench
                if (icmp_code == 0) {
                    printf("From %s: icmp_seq=%d Source Quench\n", inet_ntoa(src_addr->sin_addr), sequence);
                }
                break;

            case 5:  // Redirect
                switch (icmp_code) {
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
                        printf("From %s: icmp_seq=%d Redirect, Bad code: %d\n", inet_ntoa(src_addr->sin_addr), sequence, icmp_code);
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
                printf("From %s: icmp_seq=%d Unknown ICMP type: %d code: %d\n", inet_ntoa(src_addr->sin_addr), sequence, icmp_type, icmp_code);
                break;
        }
    }
}