.PHONY:     			all $(NAME) clear mkbuild clean fclean re

NAME					= ft_ping

BUILD_DIR				= build/

HEADER_DIR				= include/
HEADER_FILE				= ft_ping.h
HEADER_FILE_ERROR		= ft_ping_error.h

DIR						= src/
SRC			 			= main.c \
							checksum.c \
							displayStats.c \
							icmp.c \
							parsing.c \
							verbose.c \
							check_root.c \
							rtt.c \

OBJECTS			    	= $(SRC:%.c=$(BUILD_DIR)%.o)
	
GCC						= gcc
CFLAGS					= -Wall -Wextra -Werror
SANITIZE				= $(CFLAGS) -g3 -fsanitize=address

RM 						= rm -rf
CLEAR					= clear


$(BUILD_DIR)%.o: 		$(DIR)%.c $(HEADER_DIR)/$(HEADER_FILE) $(HEADER_DIR)/$(HEADER_FILE_ERROR)
						@mkdir -p $(BUILD_DIR)
						$(GCC) $(CFLAGS) -I$(HEADER_DIR) -o $@ -c $<


all: 					clear mkbuild $(HEADER_DIR) $(NAME)

						 
mkbuild:
						@mkdir -p build


clear:
						$(CLEAR)
						
$(NAME): 				$(OBJECTS)
						@$(GCC) $(OBJECTS) -o $(NAME) -lm
						@echo "$(GREEN)[✓] $(NAME) created !$(DEFAULT)"
						
clean:					
						@${RM} $(OBJECTS)
						@${RM} $(BUILD_DIR)
						@echo "$(YELLOW)[-] object files deleted !$(DEFAULT)"

fclean:					clean
						@${RM} ${NAME}
						@echo "$(RED)[x] all deleted !$(DEFAULT)"

re:						fclean all
						$(MAKE) all

RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
DEFAULT = \033[0m