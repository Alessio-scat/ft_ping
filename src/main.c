#include "../include/ft_ping.h"

int g_status = 0;

int create_raw_socket()
{
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        fprintf(stderr, ERR_CANNOT_CREATE_SOCKET, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /*
        Set a timeout for recvfrom (1 seconds)
        define a time before timeout 
    */
    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        fprintf(stderr, ERR_SET_SOCKET_TIMEOUT, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

/*
    getaddrinfo : resolve domain in IP adress
*/
void config_destination(char *hostname, struct sockaddr_in *dest_addr)
{
    struct addrinfo hints, *res;
    int errcode;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4 only
    hints.ai_socktype = SOCK_RAW; // Raw socket

    errcode = getaddrinfo(hostname, NULL, &hints, &res);
    if (errcode != 0) {
        fprintf(stderr, ERR_CANNOT_RESOLVE_HOSTNAME, hostname);
        exit(EXIT_FAILURE);
    }

    // Copy the resolved address to dest_addr
    memcpy(dest_addr, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res); // Free the linked list
}

int main(int ac, char **av) {

    char *destination = NULL;
    int verbose = 0;
    parse_command_line(ac, av, &destination, &verbose);

    int sockfd;//, verbose;
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
    int recv_any_reply = 0;
    int v = 0;

    check_root_privileges();
    sockfd = create_raw_socket();

    config_destination(destination, &dest_addr);

    printf("PING %s (%s): 56 data bytes\n", destination, inet_ntoa(dest_addr.sin_addr));
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

        int result = receive_icmp_echo_reply(sockfd, &recv_icmp_hdr, &src_addr, &ttl);
        if (result > 0)
        {
            gettimeofday(&end_time, NULL);
            calculate_and_display_rtt_statistics(&start_time, &end_time, &stats);
            double rtt = calculate_and_display_rtt(&start_time, &end_time);
            recv_any_reply = 1;
            if (verbose)
                handle_icmp_error_verbose(&recv_icmp_hdr, &src_addr, sequence, &v);
            if (v == 0)
                printf("64 bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n", destination, sequence, ttl, rtt);
        } 
        else if (result == 0){
            recv_any_reply = 2;
            printf("Request timeout for icmp_seq %d\n", sequence);
        }
        else
            fprintf(stderr, ERR_RECEIVING_ICMP_PACKET, strerror(errno));

        sequence++;
        sleep(1);
    }

    if (recv_any_reply == 1)
        calculate_and_display_statistics(&stats, 0);
    else if (recv_any_reply == 2){
        g_status = 2;
        calculate_and_display_statistics(&stats, 1);
    }
    else
        g_status = 1;

    close(sockfd);
    return g_status;
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
        - Verify if the checksum is ok
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
