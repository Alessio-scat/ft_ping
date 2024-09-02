#include "../include/ft_ping.h"

void print_help()
{
	printf("Usage: ping [OPTION...] HOST ...\n"
	       "Send ICMP ECHO_REQUEST packets to network hosts.\n\n"
	       " Options:\n"
	       "  -?                 Show help\n"
	       "  -v                 Verbose output\n");
}

void parse_command_line(int ac, char **av, char **destination) {
    int opt;
    int destination_set = 0;

    for (int i = 1; i < ac; i++) {
        if (av[i][0] == '-') {
            opt = av[i][1];
            switch (opt) {
                case 'v':
                    printf("Mode verbose activate.\n");
                    break;

                case 'h':
                    print_help();
                    break;

                default:
                    fprintf(stderr, "Ping: invalid TOS: `%c`\n", opt);
                    exit(EXIT_FAILURE);
            }
        } else if (!destination_set) {
            *destination = av[i];
            destination_set = 1;
        } else {
            fprintf(stderr, "Error: Invalid argument `%s` after destination.\n", av[i]);
            exit(EXIT_FAILURE);
        }
    }

    if (!destination_set) {
        fprintf(stderr, "Error: No destination provided.\n");
        print_help();
    }

    printf("Destination: %s\n", *destination);
    // exit(EXIT_SUCCESS);
}