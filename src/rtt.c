#include "../include/ft_ping.h"

double calculate_and_display_rtt(struct timeval *start, struct timeval *end) {
    long seconds = end->tv_sec - start->tv_sec;
    long useconds = end->tv_usec - start->tv_usec;

    double mtime = ((seconds) * 1000 + useconds / 1000.0);

    return mtime;
}