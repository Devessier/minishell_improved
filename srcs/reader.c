/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:41:45 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/11 18:49:53 by bdevessi         ###   ########.fr       */
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
		.len = 0,
		.cap = 0,
		.str = NULL
	};
}

int		sh_command_cat(t_command *this, t_sh *sh, char c)
{
	const size_t	remaining = this->cap - this->len;
	char			*tmp;
	size_t			cap;

	if (remaining < 1)
	{
		cap = this->cap == 0 ? 10 : this->cap;
		tmp = this->str;
		cap *= cap > 1024 ? 1.5 : 2;
		if (!(this->str = ft_strnew(cap)))
			return (-1);
		this->cap = cap;
		if (tmp != NULL)
			ft_strcpy(this->str, tmp);
		free(tmp);
	}
	if (sh->cursor.x == this->len)
		this->str[this->len++] = c;
	else
	{
		ft_memmove(this->str + sh->cursor.x + 1, this->str + sh->cursor.x, this->len - sh->cursor.x);
		this->str[sh->cursor.x] = c;
		this->len++;
	}
	sh->cursor.x++;
	return (0);
}

void	sh_command_del(t_command *cmd)
{
	if (cmd->len > 0)
		cmd->str[--cmd->len] = '\0';
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
