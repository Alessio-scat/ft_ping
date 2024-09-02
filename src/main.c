#include "../include/ft_ping.h"

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

double calculate_and_display_rtt(struct timeval *start, struct timeval *end) {
    long seconds = end->tv_sec - start->tv_sec;
    long useconds = end->tv_usec - start->tv_usec;

    double mtime = ((seconds) * 1000 + useconds / 1000.0);

    return mtime;
}


int main(int ac, char **av) {

    char *destination = NULL;
    parse_command_line(ac, av, &destination);

    int sockfd;//, verbose;
    // char *destination = "8.8.8.8"; // test Google
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

    printf("PING %s (%s): 56 data bytes\n", destination, destination);
    signal(SIGINT, handle_interrupt);

    while (running)
    {
        // Build Packet ICMP
        construct_icmp_echo_request(&icmp_hdr, 1);

        // share time to RTT
        gettimeofday(&start_time, NULL);

        // Send ICMP Packet
        send_icmp_echo_request(sockfd, &dest_addr, &icmp_hdr);
        stats.packets_sent++;

        if (receive_icmp_echo_reply(sockfd, &recv_icmp_hdr, &src_addr, &ttl) > 0)
        {
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
