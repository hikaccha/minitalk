# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/02 15:43:39 by ichikawahik       #+#    #+#              #
#    Updated: 2025/08/02 15:43:40 by ichikawahik      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_CLIENT	= client
NAME_SERVER	= server

SRCS_CLIENT	= srcs/client.c
SRCS_SERVER	= srcs/server.c

OBJS_CLIENT	= $(SRCS_CLIENT:.c=.o)
OBJS_SERVER	= $(SRCS_SERVER:.c=.o)

CC		= cc
CFLAGS	= -Wall -Wextra -Werror

LIBFT_PATH	= libft/
LIBFT		= $(LIBFT_PATH)libft.a

all: $(NAME_CLIENT) $(NAME_SERVER)

$(NAME_CLIENT): $(OBJS_CLIENT) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME_CLIENT) $(OBJS_CLIENT) -L$(LIBFT_PATH) -lft

$(NAME_SERVER): $(OBJS_SERVER) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME_SERVER) $(OBJS_SERVER) -L$(LIBFT_PATH) -lft

$(LIBFT):
	make -C $(LIBFT_PATH)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS_CLIENT) $(OBJS_SERVER)
	make -C $(LIBFT_PATH) clean

fclean: clean
	rm -f $(NAME_CLIENT) $(NAME_SERVER)
	make -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all clean fclean re 