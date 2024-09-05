#include "../include/ft_ping.h"

/*
    RFC 792 : Internet Control Message Protocole
    https://datatracker.ietf.org/doc/html/rfc792
*/

// void handle_icmp_error_verbose(void *recv_icmp_hdr, struct sockaddr_in *src_addr, int sequence, int *v)
// {
//     #ifdef __APPLE__
//         struct icmp *hdr = (struct icmp *)recv_icmp_hdr;  // Cast vers struct icmp pour macOS
//     #else
//         struct icmphdr *hdr = (struct icmphdr *)recv_icmp_hdr;  // Cast vers struct icmphdr pour Linux
//     #endif

//     // Vérification du type d'ICMP pour ne pas traiter les ECHO_REPLY
//     #ifdef __APPLE__
//         if (hdr->icmp_type != ICMP_ECHOREPLY) {
//     #else
//         if (hdr->type != ICMP_ECHOREPLY) {
//     #endif
//             *v = 1;

//             // Gestion des différents types d'erreurs ICMP
//     #ifdef __APPLE__
//             switch (hdr->icmp_type) {
//                 case 3:  // Destination Unreachable
//                     switch (hdr->icmp_code) {
//     #else
//             switch (hdr->type) {
//                 case 3:  // Destination Unreachable
//                     switch (hdr->code) {
//     #endif
//                         case 0:
//                             printf("From %s: icmp_seq=%d Destination Network Unreachable\n", inet_ntoa(src_addr->sin_addr), sequence);
//                             break;
//                         case 1:
//                             printf("From %s: icmp_seq=%d Destination Host Unreachable\n", inet_ntoa(src_addr->sin_addr), sequence);
//                             break;
//                         case 2:
//                             printf("From %s: icmp_seq=%d Destination Protocol Unreachable\n", inet_ntoa(src_addr->sin_addr), sequence);
//                             break;
//                         case 3:
//                             printf("From %s: icmp_seq=%d Destination Port Unreachable\n", inet_ntoa(src_addr->sin_addr), sequence);
//                             break;
//                         case 13:
//                             printf("From %s: icmp_seq=%d Communication administratively prohibited\n", inet_ntoa(src_addr->sin_addr), sequence);
//                             break;
//                         default:
//     #ifdef __APPLE__
//                             printf("From %s: icmp_seq=%d Destination Unreachable, Bad code: %d\n", inet_ntoa(src_addr->sin_addr), sequence, hdr->icmp_code);
//     #else
//                             printf("From %s: icmp_seq=%d Destination Unreachable, Bad code: %d\n", inet_ntoa(src_addr->sin_addr), sequence, hdr->code);
//     #endif
//                             break;
//                     }
//                     break;

//                 case 11:  // Time Exceeded
//     #ifdef __APPLE__
//                     switch (hdr->icmp_code) {
//     #else
//                     switch (hdr->code) {
//     #endif
//                         case 0:
//                             printf("From %s: icmp_seq=%d Time to live exceeded\n", inet_ntoa(src_addr->sin_addr), sequence);
//                             break;
//                         case 1:
//                             printf("From %s: icmp_seq=%d Fragment reassembly time exceeded\n", inet_ntoa(src_addr->sin_addr), sequence);
//                             break;
//                         default:
//     #ifdef __APPLE__
//                             printf("From %s: icmp_seq=%d Time Exceeded, Bad code: %d\n", inet_ntoa(src_addr->sin_addr), sequence, hdr->icmp_code);
//     #else
//                             printf("From %s: icmp_seq=%d Time Exceeded, Bad code: %d\n", inet_ntoa(src_addr->sin_addr), sequence, hdr->code);
//     #endif
//                             break;
//                     }
//                     break;

//                 case 12:
//     #ifdef __APPLE__
//                     switch (hdr->icmp_code) {
//     #else
//                     switch (hdr->code) {
//     #endif
//                         case 0:
//                             printf("From %s: icmp_seq=%d Pointer indicates the error\n", inet_ntoa(src_addr->sin_addr), sequence);
//                             break;
//                     }
//                     break;

//                 case 4:
//     #ifdef __APPLE__
//                     switch (hdr->icmp_code) {
//     #else
//                     switch (hdr->code) {
//     #endif
//                         case 0:
//                             printf("From %s: icmp_seq=%d Source Quench\n", inet_ntoa(src_addr->sin_addr), sequence);
//                             break;
//                     }
//                     break;

//                 case 5:  // Redirect
//     #ifdef __APPLE__
//                     switch (hdr->icmp_code) {
//     #else
//                     switch (hdr->code) {
//     #endif
//                         case 0:
//                             printf("From %s: icmp_seq=%d Redirect Network\n", inet_ntoa(src_addr->sin_addr), sequence);
//                             break;
//                         case 1:
//                             printf("From %s: icmp_seq=%d Redirect Host\n", inet_ntoa(src_addr->sin_addr), sequence);
//                             break;
//                         case 2:
//                             printf("From %s: icmp_seq=%d Redirect Network for Type of Service\n", inet_ntoa(src_addr->sin_addr), sequence);
//                             break;
//                         case 3:
//                             printf("From %s: icmp_seq=%d Redirect Host for Type of Service\n", inet_ntoa(src_addr->sin_addr), sequence);
//                             break;
//                         default:
//     #ifdef __APPLE__
//                             printf("From %s: icmp_seq=%d Redirect, Bad code: %d\n", inet_ntoa(src_addr->sin_addr), sequence, hdr->icmp_code);
//     #else
//                             printf("From %s: icmp_seq=%d Redirect, Bad code: %d\n", inet_ntoa(src_addr->sin_addr), sequence, hdr->code);
//     #endif
//                             break;
//                     }
//                     break;

//                 case 13:  // Timestamp Request
//                     printf("From %s: icmp_seq=%d Timestamp Request\n", inet_ntoa(src_addr->sin_addr), sequence);
//                     break;

//                 case 14:  // Timestamp Reply
//                     printf("From %s: icmp_seq=%d Timestamp Reply\n", inet_ntoa(src_addr->sin_addr), sequence);
//                     break;

//                 case 15:  // Information Request
//                     printf("From %s: icmp_seq=%d Information Request\n", inet_ntoa(src_addr->sin_addr), sequence);
//                     break;

//                 case 16:  // Information Reply
//                     printf("From %s: icmp_seq=%d Information Reply\n", inet_ntoa(src_addr->sin_addr), sequence);
//                     break;

//                 default:
//     #ifdef __APPLE__
//                     printf("From %s: icmp_seq=%d Unknown ICMP type: %d code: %d\n", inet_ntoa(src_addr->sin_addr), sequence, hdr->icmp_type, hdr->icmp_code);
//     #else
//                     printf("From %s: icmp_seq=%d Unknown ICMP type: %d code: %d\n", inet_ntoa(src_addr->sin_addr), sequence, hdr->type, hdr->code);
//     #endif
//                     break;
//             }
//         }
// }


#include <netinet/ip.h>
#ifdef __APPLE__
#include <netinet/ip_icmp.h>  // Pour macOS : struct icmp
typedef struct icmp icmp_t;   // Alias pour macOS
#else
#include <netinet/ip_icmp.h>  // Pour Linux : struct icmphdr
typedef struct icmphdr icmp_t;  // Alias pour Linux
#endif

void handle_icmp_error_verbose(void *recv_icmp_hdr, struct sockaddr_in *src_addr, int sequence, int *v)
{
    // int is_macos;

    // // Détection de l'OS
    // #ifdef __APPLE__
    //     is_macos = 1;
    // #else
    //     is_macos = 0;
    // #endif

    // icmp_t *hdr = (icmp_t *)recv_icmp_hdr;

    // int icmp_type = is_macos ? hdr->icmp_type : hdr->type;
    // int icmp_code = is_macos ? hdr->icmp_code : hdr->code;

    int icmp_type;
    int icmp_code;

    // Cast du pointeur recv_icmp_hdr selon le système
    #ifdef __APPLE__
        struct icmp *hdr = (struct icmp *)recv_icmp_hdr;
        icmp_type = hdr->icmp_type;
        icmp_code = hdr->icmp_code;
    #else
        struct icmphdr *hdr = (struct icmphdr *)recv_icmp_hdr;
        icmp_type = hdr->type;
        icmp_code = hdr->code;
    #endif

    if (icmp_type != ICMP_ECHOREPLY) {
        *v = 1;

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