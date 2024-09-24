#include "../include/ft_ping.h"

volatile int running = 1;
ping_stats_t stats;

void handle_interrupt(int sig) {
    (void)sig;
    running = 0;
}

void calculate_and_display_rtt_statistics(struct timeval *start, struct timeval *end, ping_stats_t *stats) {
    long seconds = end->tv_sec - start->tv_sec;
    long useconds = end->tv_usec - start->tv_usec;
    double rtt = ((seconds) * 1000 + useconds / 1000.0);

    // MAJ stats RTT
    if (stats->rtt_min < 0 || rtt < stats->rtt_min)
        stats->rtt_min = rtt;
    if (rtt > stats->rtt_max)
        stats->rtt_max = rtt;
    stats->rtt_total += rtt;
    stats->rtt_sum_of_squares += rtt * rtt;

    stats->packets_received++;
}

void calculate_and_display_statistics(ping_stats_t *stats, int tv) {
    double rtt_avg = stats->rtt_total / stats->packets_received;
    double rtt_stddev = sqrt((stats->rtt_sum_of_squares / stats->packets_received) - (rtt_avg * rtt_avg));

    printf("\n--- Ping statistics ---\n");
    printf("%d packets transmitted, %d packets received, %.1f%% packet loss, %.0f ms\n",
           stats->packets_sent, stats->packets_received,
           ((stats->packets_sent - stats->packets_received) / (double)stats->packets_sent) * 100,
            stats->total_time);
    if (tv == 0)
        printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", stats->rtt_min, rtt_avg, stats->rtt_max, rtt_stddev);
}
