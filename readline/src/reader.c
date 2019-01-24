/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 16:32:37 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/24 16:32:39 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"
#include <sys/types.h>

int     ft_rl_getchar(t_ft_rl_reader *this, const int fd, char *c)
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

int     ft_rl_getchar_blocking(t_ft_rl_reader *this, const int fd, char *c)
{
    int n;

    while ((n = ft_rl_getchar(this, fd, c)) != 1)
        if (n == -1)
            return (n);
    return (1);
}
