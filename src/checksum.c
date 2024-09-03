#include "../include/ft_ping.h"


/*
    Verify integrity data message ICMP.
    if the checksum not equals start and end => error

    @calcul: Adding all world 16 bytes packet ICMP 
*/

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