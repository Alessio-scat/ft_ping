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

int receive_icmp_echo_reply(int sockfd, void *recv_icmp_hdr, struct sockaddr_in *src_addr, uint8_t *ttl, 
                             #ifdef __APPLE__
                               struct ip *ip_hdr_copy
                               #else
                               struct iphdr *ip_hdr_copy
                               #endif
                               )
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
        memcpy(ip_hdr_copy, ip_hdr, sizeof(struct ip));
    #else
        struct iphdr *ip_hdr = (struct iphdr *)buffer;
        memcpy(ip_hdr_copy, ip_hdr, sizeof(struct iphdr));
    #endif

    #ifdef __APPLE__
        int ip_hdr_len = ip_hdr->ip_hl * 4;
    #else
        int ip_hdr_len = ip_hdr->ihl * 4;
    #endif

    #ifdef __APPLE__
        struct icmp *hdr = (struct icmp *)(buffer + ip_hdr_len);
    #else
        struct icmphdr *hdr = (struct icmphdr *)(buffer + ip_hdr_len);
    #endif

        memcpy(recv_icmp_hdr, hdr, sizeof(*hdr));

    #ifdef __APPLE__
        *ttl = ip_hdr->ip_ttl;
    #else
        *ttl = ip_hdr->ttl;
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

    #ifdef __APPLE__
        if (hdr->icmp_type == ICMP_ECHOREPLY)
            if (ntohs(hdr->icmp_id) == (getpid() & 0xFFFF))
                return 1;
            else
                return 0; 
        else if (hdr->icmp_type == 8)
            return 1;
        else if (hdr->icmp_type == 3 ||  hdr->icmp_type == 11 || hdr->icmp_type == 12 || hdr->icmp_type == 4 || hdr->icmp_type == 5)
            return -1;

    #else
        if (hdr->type == ICMP_ECHOREPLY)
            if (ntohs(hdr->un.echo.id) == (getpid() & 0xFFFF))
                return 1;
            else
                return 0;
        else if (hdr->type == 8)
            return 1;
        else if (hdr->type == 3 ||  hdr->type == 11 || hdr->type == 12 || hdr->type == 4 || hdr->type == 5)
            return -1;
    #endif

    return 0;
}