# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/06 16:01:17 by bdevessi          #+#    #+#              #
#    Updated: 2019/02/21 13:45:28 by bdevessi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CFLAGS = -Wall -Wextra -Werror -Iinclude -Ilibft/includes -Ireadline/include -g
CC = gcc 

include src.mk

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(OBJS): Makefile include/sh.h libft/libft.a readline/readline.a

$(NAME): $(OBJS)
	$(MAKE) -C readline readline.a
	$(MAKE) -C libft libft.a
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) libft/libft.a readline/readline.a

clean:
	rm -f $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME)

.PHONY: clean fclean re proto
