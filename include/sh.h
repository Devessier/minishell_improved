/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:29:24 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/11 10:51:12 by bdevessi         ###   ########.fr       */
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

typedef struct	s_command
{
	size_t	len;
	size_t	cap;
	char	*str;
}				t_command;

void			reader_init(t_reader *this);
void			command_init(t_reader *this;
int				sh_command_cat(t_command *this, char c);
ssize_t			sh_getchar(t_reader *this, const int fd, char *c);

#endif
