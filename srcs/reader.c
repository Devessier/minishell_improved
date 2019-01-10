/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:41:45 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/10 23:46:39 by bdevessi         ###   ########.fr       */
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
