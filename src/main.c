#include "../include/ft_ping.h"

// volatile int running = 1;

// void handle_interrupt(int sig) {
//     (void)sig;
//     running = 0;
// }

void check_root_privileges() {
    if (getuid() != 0) {
        fprintf(stderr, "Error: this program must be run as root.\n");
        exit(EXIT_FAILURE);
    }
}

int create_raw_socket()
{
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("Error creating raw socket");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

// To calculate checksum
unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

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
        perror("Error sending ICMP packet");
        exit(EXIT_FAILURE);
    }
}

double calculate_and_display_rtt(struct timeval *start, struct timeval *end) {
    long seconds = end->tv_sec - start->tv_sec;
    long useconds = end->tv_usec - start->tv_usec;

    double mtime = ((seconds) * 1000 + useconds / 1000.0);

    printf("RTTTTTTTTTTT: %f ms\n", mtime);

    return mtime;
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
        perror("Error receiving ICMP packet");
        return -1;
    }

    struct iphdr *ip_hdr = (struct iphdr *)buffer;
    int ip_hdr_len = ip_hdr->ihl * 4; // ihl (Internet Header Length)
    memcpy(recv_icmp_hdr, buffer + ip_hdr_len, sizeof(struct icmphdr));

    *ttl = ip_hdr->ttl;
    printf("TTL: %d\n", *ttl);

    // memcpy(recv_icmp_hdr, buffer, sizeof(struct icmphdr));

    return (recv_icmp_hdr->type == ICMP_ECHOREPLY) && (recv_icmp_hdr->un.echo.id == getpid());
}

int main() {
    int sockfd;//, verbose;
    char *destination = "8.8.8.8"; // test Google
    struct sockaddr_in dest_addr, src_addr; // IP Adress dest and src
    struct icmphdr icmp_hdr, recv_icmp_hdr; // Build and Analyse packets ICMP
    struct timeval start_time, end_time; // time
    int sequence = 0;
    stats.packets_sent = 0;
    stats.packets_received = 0;
    stats.rtt_min = -1;
    stats.rtt_max = 0;
    stats.rtt_total = 0;
    stats.rtt_sum_of_squares = 0;
    u_int8_t ttl;

    check_root_privileges();
    sockfd = create_raw_socket();

    // Config destination addr
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, destination, &dest_addr.sin_addr) <= 0) {
        perror("IP address conversion error");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, handle_interrupt);

    while (running)
    {
        // Build Packet ICMP
        construct_icmp_echo_request(&icmp_hdr, 1);

        // share time to RTT
        gettimeofday(&start_time, NULL);

        // Send ICMP Packet
        send_icmp_echo_request(sockfd, &dest_addr, &icmp_hdr);
        printf("ICMP Packet Send.\n");
        stats.packets_sent++;

        printf("Waiting of receive ICMP Packet...\n");
        if (receive_icmp_echo_reply(sockfd, &recv_icmp_hdr, &src_addr, &ttl) > 0)
        {
            printf("ICMP Packet receive.\n");
            gettimeofday(&end_time, NULL);
            calculate_and_display_rtt_statistics(&start_time, &end_time, &stats);
            double rtt = calculate_and_display_rtt(&start_time, &end_time);
            printf("64 bytes form %s: icmp_seq=%d ttl=%d time=%.3f ms\n", destination, sequence, ttl, rtt);
        }
        else
            printf("Nothing packet ICMP receive.\n");

        sequence++;
        sleep(1);
    }

    close(sockfd);
    return 0;
}


/*
    Step1 : Create Raw socket

        - Create raw socket that allow to send and receive ICMP packets
        - Verify to execute this to be privilege root

    Step2 : Build Message ICMP ECHO Request

        - Create a fuction that build a packet ICMP ECHO Request
        - Include fields : Type, Code, Checksum, Identifier, Sequence Number and Data
        - Calculate the checksum carefully, because it's very important

    Step3 : Send message ICMP

        - Use the raw socket to send packet ICMP ECHO Request to target ip
        - Manage error if the send fail

    Step4 : Receive Response ICMP ECHO Reply

        - Implement reception packets ICMP ECHO Reply
        - Verify if the responde correspond the request send used fields Identifier and Sequence Number
        - if the message Echo Reply is not receive, manage this 

    Step5 : Calculate and Display RTT

        - Share time before to send ECHO Request
        - When I receive ECHO Reply, calcule the difference to obtain RTT
        - Display thi RTT for each responses

    Step6 : Manage Option 

        - -v : Verbose mode 
        - -? : Displah help

    Step7 : Bonus...

*/