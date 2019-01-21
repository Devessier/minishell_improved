/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:41:45 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/21 14:31:34 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

void	reader_init(t_reader *this)
{
	*this = (t_reader) {
		.len = 0,
		.index = 0
	};
}

void	command_init(t_command *this)
{
	*this = (t_command) {
		.cursor = 0,
		.string = (t_string) {
			.len = 0,
			.cap = 0,
			.str = NULL
		}
	};
}

int		sh_command_cat(t_command *this, t_sh *sh, char c)
{
	if (!extend_string(&this->string, 1))
		return (-1);
	if (sh->cursor.index == this->string.len)
		this->string.str[this->string.len++] = c;
	else
	{
		ft_memmove(this->string.str + sh->cursor.index + 1,
			this->string.str + sh->cursor.index, this->string.len - sh->cursor.index);
		this->string.str[sh->cursor.index] = c;
		this->string.len++;
	}
	return (0);
}

void	sh_command_del(t_command *this, t_sh *sh, bool current)
{
	if (this->string.len <= 0 || (current && sh->cursor.index <= 0))
		return ft_putchar(0x7);
	ft_strcpy(this->string.str + sh->cursor.index + (current ? -1 : 0),
		this->string.str + sh->cursor.index + (current ? 0 : 1));
	this->string.len--;
	if (current)
		move_cursor(this, sh, -1);
}

ssize_t	sh_getchar(t_reader *this, const int fd, char *c)
{
	const size_t	remaining = this->len - this->index;
	size_t			buffered;

	if (remaining < 1)
	{
		if ((this->len = read(fd, this->buffer, sizeof(this->buffer))) < 0)
			return (-1);
		this->index = 0;
		buffered = this->len;
	}
	else
		buffered = this->len - this->index;
	if (this->len == 0)
		return (0);
	*c = this->buffer[this->index++];
	return (1);
}
