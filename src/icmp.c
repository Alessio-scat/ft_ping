#include "../include/ft_ping.h"

void construct_icmp_echo_request(struct icmphdr *icmp_hdr, int sequence) {
    memset(icmp_hdr, 0, sizeof(struct icmphdr));
    icmp_hdr->type = ICMP_ECHO; // It's 8 to ECHO_REQUEST 
    icmp_hdr->code = 0; // Always to ECHO_REQUEST 
    icmp_hdr->un.echo.id = getpid();
    icmp_hdr->un.echo.sequence = sequence;
    icmp_hdr->checksum = checksum(icmp_hdr, sizeof(struct icmphdr));
}

/*
    sendto : send data throught socket 
        - 0 : No flag
*/
void send_icmp_echo_request(int sockfd, struct sockaddr_in *dest_addr, struct icmphdr *icmp_hdr) {
    if (sendto(sockfd, icmp_hdr, sizeof(struct icmphdr), 0, (struct sockaddr *)dest_addr, sizeof(*dest_addr)) <= 0) {
        fprintf(stderr, ERR_SENDTO_NETWORK_UNREACHABLE);
        exit(EXIT_FAILURE);
    }
}

/*
    @buffer : stock temporary data of network
    @addr_len : stock len struct sockaddr_in

    ignore the IP header (which is always present in a network packet) and concentrate solely on the ICMP header
*/

int receive_icmp_echo_reply(int sockfd, struct icmphdr *recv_icmp_hdr, struct sockaddr_in *src_addr, uint8_t *ttl) {
    char buffer[1024];
    socklen_t addr_len = sizeof(*src_addr);

    int bytes_received = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)src_addr, &addr_len);
    if (bytes_received < 0) {
        fprintf(stderr, ERR_REQUEST_TIMEOUT);
        return -1;
    }

    struct iphdr *ip_hdr = (struct iphdr *)buffer;
    int ip_hdr_len = ip_hdr->ihl * 4; // ihl (Internet Header Length)
    memcpy(recv_icmp_hdr, buffer + ip_hdr_len, sizeof(struct icmphdr));

    *ttl = ip_hdr->ttl;

    unsigned short received_checksum = recv_icmp_hdr->checksum;
    recv_icmp_hdr->checksum = 0;
    unsigned short calculated_checksum = checksum(recv_icmp_hdr, sizeof(struct icmphdr));
    

    if (received_checksum != calculated_checksum) {
        fprintf(stderr, ERR_BAD_CHECKSUM);
        return -1;
    }

    return (recv_icmp_hdr->type == ICMP_ECHOREPLY) && (recv_icmp_hdr->un.echo.id == getpid());
}