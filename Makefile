# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/06 16:01:17 by bdevessi          #+#    #+#              #
#    Updated: 2019/03/07 12:49:51 by bdevessi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell
CFLAGS = -Wall -Wextra -Werror -Iinclude -Ilibft/includes -Ireadline/include
CC = gcc

include src.mk

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(OBJS): Makefile src.mk include/sh.h

$(NAME): $(OBJS)
	$(MAKE) -C libft
	$(MAKE) -C readline
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) libft/libft.a readline/readline.a

clean:
	$(MAKE) -C libft clean
	$(MAKE) -C readline clean
	rm -f $(OBJS)

fclean: clean
	rm -rf libft/libft.a
	rm -rf readline/readline.a
	rm -rf $(NAME)

re: fclean $(NAME)

.PHONY: clean fclean re proto
