#include "../include/ft_ping.h"

void print_help()
{
	printf("Usage: ping [OPTION...] HOST ...\n"
	       "Send ICMP ECHO_REQUEST packets to network hosts.\n\n"
	       " Options:\n"
	       "  -h or -?           Show help\n"
	       "  -v                 Verbose output\n");
        
    exit(EXIT_SUCCESS);
}

int is_valid_ipv4(const char *ip)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ip, &(sa.sin_addr));

    if (result == 1)
        return 1;
    else    
        return 0;
}

int is_valid_hostname(const char *hostname) {

    const char *regex_pattern = "^[a-zA-Z0-9]([a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(\\.[a-zA-Z0-9]([a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$";
    regex_t regex;
    int ret;
    ret = regcomp(&regex, regex_pattern, REG_EXTENDED);

    if (ret) {
        fprintf(stderr, ERR_REGEX_COMPILATION);
        exit(EXIT_FAILURE);
    }

    ret = regexec(&regex, hostname, 0, NULL, 0);
    regfree(&regex);
    
    if (ret == 0)
        return 1;
    else
        return 0;

}

void parse_command_line(int ac, char **av, char **destination, int *verbose) {
    int opt;
    int destination_set = 0;

    for (int i = 1; i < ac; i++) {
        if (av[i][0] == '-') {
            opt = av[i][1];
            switch (opt) {
                case 'v':
                    if (av[i][2] != '\0'){
                        print_help();
                        exit(EXIT_FAILURE);
                    }
                    *verbose = 1;
                    break;

                case 'h':
                    print_help();
                    break;

                case '?':
                    print_help();
                    break;

                default:
                    fprintf(stderr, ERR_INVALID_TOS, opt);
                    exit(EXIT_FAILURE);
            }
        } else if (!destination_set) {
            *destination = av[i];
            destination_set = 1;
        } else {
            fprintf(stderr, ERR_INVALID_ARGUMENT, av[i]);
            exit(EXIT_FAILURE);
        }
    }

    if (!destination_set) {
        fprintf(stderr, ERR_NO_DESTINATION);
        print_help();
    }

    // Validate the destination

    if (is_valid_ipv4(*destination) == 1) {} 
    else if (is_valid_hostname(*destination) == 1) {} 
    else {
        fprintf(stderr, ERR_INVALID_DESTINATION, *destination);
        exit(EXIT_FAILURE);
    }

}