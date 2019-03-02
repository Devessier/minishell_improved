/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 16:32:37 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/02 12:50:03 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"
#include <sys/types.h>
#include <unistd.h>

int		ft_rl_getchar(t_ft_rl_reader *this, const int fd, char *c)
{
	const int		stdin_tty = isatty(0);
	const size_t	remaining = this->len - this->index;
	size_t			buffered;

	if (remaining < 1)
	{
		if ((this->len = read(fd, this->buffer, sizeof(this->buffer))) < 0)
			return (-1);
		if (this->len == 0 && !stdin_tty)
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

int		ft_rl_getchar_blocking(t_ft_rl_reader *this
		, const int fd, char *c)
{
	int n;

	while ((n = ft_rl_getchar(this, fd, c)) != 1)
		if (n == -1)
			return (n);
	return (n == 1 ? 1 : -1);
}

void	init_ft_rl_reader_string(t_ft_rl_reader *reader,
	t_string *string, char characters[2])
{
	*reader = (t_ft_rl_reader) {
		.len = 0,
		.index = 0,
	};
	ft_bzero(reader->buffer, sizeof(reader->buffer));
	*string = (t_string) {
		.len = 0,
		.cap = 0,
		.buff = NULL,
	};
	ft_bzero(characters, sizeof(*characters) * 2);
}
