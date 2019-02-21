/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 11:20:54 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/21 11:18:13 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

t_string	ft_new_string(char *c, bool char_mode)
{
	t_string	string;

	string = (t_string) { 0, 0, NULL };
	if (*c == '\0')
		return (string);
	string.len = char_mode ? 1 : ft_strlen(c);
	string.cap = 10 < string.len ? string.len * 2 : 10;
	if (!(string.buff = ft_strnew(string.cap)))
		return ((t_string) { 0, 0, NULL });
	if (char_mode)
		*string.buff = *c;
	else
		ft_strcat(string.buff, c);
	return (string);
}

bool		ft_concat_strings(t_string *this, char *string, size_t len)
{
	size_t			cap;
	char			*tmp;

	cap = this->cap == 0 ? len : this->cap;
	if (this->len + len > this->cap)
	{
		if (this->len + len > cap * 2)
			cap += (this->len + len) * 2;
		else
			cap *= 2;
		if (!(tmp = ft_strnew(cap)))
			return (false);
		if (string != NULL && this->len > 0)
			ft_strcat(tmp, this->buff);
		this->cap = cap;
		free(this->buff);
		this->buff = tmp;
	}
	if (string != NULL)
	{
		ft_strncat(this->buff, string, len);
		this->len += len;
	}
	return (true);
}

bool		ft_extend_string(t_string *this, size_t size)
{
	char	*tmp;
	size_t	cap;

	cap = this->cap;
	if (this->len + size > this->cap)
	{
		cap = ft_round_up_2n_pow(cap + size);
		tmp = this->buff;
		if (!(this->buff = ft_strnew(cap)))
			return (false);
		this->cap = cap;
		if (tmp)
			ft_strcpy(this->buff, tmp);
		free(tmp);
	}
	return (true);
}

bool		ft_free_string(t_string *this)
{
	free(this->buff);
	this->buff = NULL;
	this->len = 0;
	this->cap = 0;
	return (true);
}

void		ft_putnstring(t_string *this, size_t start, size_t n)
{
	write(1, this->buff + start, ft_min(this->len, n));
}
