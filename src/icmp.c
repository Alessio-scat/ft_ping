#include "../include/ft_ping.h"

void construct_icmp_echo_request(void *icmp_hdr, int sequence)
{
#ifdef __APPLE__
    struct icmp *hdr = (struct icmp *)icmp_hdr;
    hdr->icmp_type = ICMP_ECHO;
    hdr->icmp_code = 0;
    hdr->icmp_id = htons(getpid() & 0xFFFF);
    hdr->icmp_seq = htons(sequence);
    hdr->icmp_cksum = 0;
    hdr->icmp_cksum = checksum(hdr, sizeof(struct icmp));
#else
    struct icmphdr *hdr = (struct icmphdr *)icmp_hdr;
    hdr->type = ICMP_ECHO;
    hdr->code = 0;
    hdr->un.echo.id = htons(getpid() & 0xFFFF);
    hdr->un.echo.sequence = htons(sequence);
    hdr->checksum = 0;
    hdr->checksum = checksum(hdr, sizeof(struct icmphdr));
#endif
}

// void construct_icmp_echo_request(struct icmphdr *icmp_hdr, int sequence) {
//     memset(icmp_hdr, 0, sizeof(struct icmphdr));
//     icmp_hdr->type = ICMP_ECHO; // It's 8 to ECHO_REQUEST 
//     icmp_hdr->code = 0; // Always to ECHO_REQUEST 
//     printf("1)ft_construct_icmp_echo_request() --> %d\n", icmp_hdr->un.echo.id);
//     icmp_hdr->un.echo.id = getpid();//pid;
//     printf("2)ft_construct_icmp_echo_request() --> %d\n", icmp_hdr->un.echo.id);
//     icmp_hdr->un.echo.sequence = sequence;
//     icmp_hdr->checksum = checksum(icmp_hdr, sizeof(struct icmphdr));
//     printf("Packet sent: ID = %d, Seq = %d, Checksum = %d\n", icmp_hdr->un.echo.id, icmp_hdr->un.echo.sequence, icmp_hdr->checksum);
// }

/*
    sendto : send data throught socket 
        - 0 : No flag
*/
// void send_icmp_echo_request(int sockfd, struct sockaddr_in *dest_addr, struct icmphdr *icmp_hdr) {
//     if (sendto(sockfd, icmp_hdr, sizeof(struct icmphdr), 0, (struct sockaddr *)dest_addr, sizeof(*dest_addr)) <= 0) {
//         fprintf(stderr, ERR_SENDTO_NETWORK_UNREACHABLE);
//         exit(EXIT_FAILURE);
//     }
// }

void send_icmp_echo_request(int sockfd, struct sockaddr_in *dest_addr, void *icmp_hdr)
{
#ifdef __APPLE__
    if (sendto(sockfd, icmp_hdr, sizeof(struct icmp), 0, (struct sockaddr *)dest_addr, sizeof(*dest_addr)) <= 0) {
#else
    if (sendto(sockfd, icmp_hdr, sizeof(struct icmphdr), 0, (struct sockaddr *)dest_addr, sizeof(*dest_addr)) <= 0) {
#endif
        fprintf(stderr, ERR_SENDTO_NETWORK_UNREACHABLE);
        exit(EXIT_FAILURE);
    }
}


/*
    @buffer : stock temporary data of network
    @addr_len : stock len struct sockaddr_in

    ignore the IP header (which is always present in a network packet) and concentrate solely on the ICMP header
*/

// int receive_icmp_echo_reply(int sockfd, struct icmphdr *recv_icmp_hdr, struct sockaddr_in *src_addr, uint8_t *ttl) {
//     char buffer[1024];
//     socklen_t addr_len = sizeof(*src_addr);

//     printf("1)ft_receive_icmp_echo_reply() --> %d\n", recv_icmp_hdr->un.echo.id);
//     int bytes_received = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)src_addr, &addr_len);
//     if (bytes_received < 0) {
//         if (errno == EAGAIN || errno == EWOULDBLOCK) {
//             // Timeout occurred
//             return 0;
//         } else {
//             fprintf(stderr, ERR_RECEIVING_ICMP_PACKET, strerror(errno));
//             return -1;
//         }
//     }

//     struct iphdr *ip_hdr = (struct iphdr *)buffer;
//     int ip_hdr_len = ip_hdr->ihl * 4; // ihl (Internet Header Length)
//     memcpy(recv_icmp_hdr, buffer + ip_hdr_len, sizeof(struct icmphdr));

//     *ttl = ip_hdr->ttl;

//     unsigned short received_checksum = recv_icmp_hdr->checksum;
//     recv_icmp_hdr->checksum = 0;
//     unsigned short calculated_checksum = checksum(recv_icmp_hdr, sizeof(struct icmphdr));
//     printf("2)ft_receive_icmp_echo_reply() --> %d\n", recv_icmp_hdr->un.echo.id);
    

//     if (received_checksum != calculated_checksum) {
//         fprintf(stderr, ERR_BAD_CHECKSUM);
//         return -1;
//     }

//     printf("Packet received: ID = %d, Seq = %d, Checksum = %d\n", recv_icmp_hdr->un.echo.id, recv_icmp_hdr->un.echo.sequence, received_checksum);


//     // printf("PID DU MAIN : %d\n", pid);
//     // int i = getpid();
//     // printf("GETPID() : %d\n", getpid());
//     // printf("GETPID() I : %d\n", i);

//     return (recv_icmp_hdr->type == ICMP_ECHOREPLY) && (recv_icmp_hdr->un.echo.id == getpid());
// }

int receive_icmp_echo_reply(int sockfd, void *recv_icmp_hdr, struct sockaddr_in *src_addr, uint8_t *ttl)
{
    char buffer[1024];
    socklen_t addr_len = sizeof(*src_addr);
    int bytes_received = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)src_addr, &addr_len);
    if (bytes_received < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return 0;  // Timeout occurred
        } else {
            fprintf(stderr, ERR_RECEIVING_ICMP_PACKET, strerror(errno));
            return -1;
        }
    }

    #ifdef __APPLE__
        struct ip *ip_hdr = (struct ip *)buffer;
    #else
        struct iphdr *ip_hdr = (struct iphdr *)buffer;
    #endif

        int ip_hdr_len = ip_hdr->ip_hl * 4;  // Utilisez ip_hl sur macOS et ihl sur Linux

    #ifdef __APPLE__
        struct icmp *hdr = (struct icmp *)(buffer + ip_hdr_len);
    #else
        struct icmphdr *hdr = (struct icmphdr *)(buffer + ip_hdr_len);
    #endif

        memcpy(recv_icmp_hdr, hdr, sizeof(*hdr));

    #ifdef __APPLE__
        *ttl = ip_hdr->ip_ttl;  // Utilisez ip_ttl sur macOS
    #else
        *ttl = ip_hdr->ttl;  // Utilisez ttl sur Linux
    #endif

    #ifdef __APPLE__
        unsigned short received_checksum = hdr->icmp_cksum;
        hdr->icmp_cksum = 0;
        unsigned short calculated_checksum = checksum(hdr, sizeof(struct icmp));
    #else
        unsigned short received_checksum = hdr->checksum;
        hdr->checksum = 0;
        unsigned short calculated_checksum = checksum(hdr, sizeof(struct icmphdr));
    #endif

    if (received_checksum != calculated_checksum) {
        fprintf(stderr, ERR_BAD_CHECKSUM);
        return -1;
    }

    return (hdr->icmp_type == ICMP_ECHOREPLY) && (ntohs(hdr->icmp_id) == (getpid() & 0xFFFF));
}