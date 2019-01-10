/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:29:24 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/10 23:44:00 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_H
# define SH_H
# include <types.h>

typedef struct	s_reader
{
	char	buffer[ARG_MAX];
	size_t	len;
	size_t	index;
}				t_reader;

void			reader_init(t_reader *this);
ssize_t			reader_getchar(t_reader *this, const int fd, char *c);

#endif
