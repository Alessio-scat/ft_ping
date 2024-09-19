#include "../include/ft_ping.h"

/*
    Definition here
    @volatile : value must changed
*/
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
    printf("%d packets transmitted, %d packets received, %.1f%% packet loss, %.4g ms\n",
           stats->packets_sent, stats->packets_received,
           ((stats->packets_sent - stats->packets_received) / (double)stats->packets_sent) * 100,
            stats->total_time);
    if (tv == 0)
        printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", stats->rtt_min, rtt_avg, stats->rtt_max, rtt_stddev);
}

/*
    1. Titre :
        --- Ping statistics ---
            - Ce titre indique que les informations suivantes sont un résumé des statistiques de l'opération ping.
    
    2. Statistiques des Paquets :
        X packets transmitted
            - Nombre total de paquets ICMP Echo Request envoyés à la destination.
        Y packets received
            - Nombre total de paquets ICMP Echo Reply reçus en réponse aux requêtes envoyées.
        Z% packet loss
            - Pourcentage de paquets perdus. C'est le ratio de paquets envoyés qui n'ont pas reçu de réponse. Un taux de 0.0% indique que tous les paquets envoyés ont été reçus.
    
    3. Temps de Réponse Aller-Retour (RTT) :
        round-trip min/avg/max/stddev = A/B/C/D ms
            - min : Le temps de réponse minimum enregistré pour un paquet.
            - avg : Le temps de réponse moyen calculé pour tous les paquets reçus.
            - max : Le temps de réponse maximum enregistré pour un paquet.
            - stddev : L'écart-type des temps de réponse, mesurant la variation autour de la moyenne. Un écart-type faible indique des temps de réponse cohérents.
*/