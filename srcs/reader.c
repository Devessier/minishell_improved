/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:41:45 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/11 10:50:48 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <stdlib.h>
#include <types.h>

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

int		sh_command_cat(t_command *this, char c)
{
	const size_t	remaining = this->cap - this->len;
	char			*tmp;
	size_t			cap;

	if (remaining < 1)
	{
		cap = this->cap == 0 ? 10 : this->cap;
		tmp = this->str;
		cap *= cap > 1024 ? 1.5 : 2;
		if (this->str = ft_strnew(cap))
			return (-1);
		this->cap = cap;
		if (tmp != NULL)
			ft_strcpy(this->str, tmp);
		free(tmp);
	}
	this->str[this->len++] = c;
	return (0);
}

ssize_t	sh_getchar(t_reader *this, const int fd, char *c)
{
	// la fonction doit set à chaque appel le caractère suivant dans le paramètre c
	// si l'on atteint la limite du buffer ou que le caractère suivant est un \n (fin de la commande => exécution) on remplit le champ `cmd` de la structure t_reader
	const size_t	remaining = this->len - this->index;
	size_t			nbytes;

	if (remaining == 0)
	{
		*c = self->buffer[self->index]
	}
}
