/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 11:25:30 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/16 12:36:17 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <stdbool.h>
#include <stdio.h>

t_string	new_string(char *c, bool char_mode)
{
	t_string	string;

	string = (t_string) { 0, 0, NULL };
	if (*c == '\0')
		return (string);
	string.len = char_mode ? 1 : ft_strlen(c);
	string.cap = 10 < string.len ? string.len * 2 : 10;
	if (!(string.str = ft_strnew(string.cap)))
		return ((t_string) { 0, 0, NULL });
	if (char_mode)
		*string.str = *c;
	else
		ft_strcat(string.str, c);
	return (string);
}

bool		extend_string(t_string *this, size_t size)
{
	char	*tmp;
	size_t	cap;

	cap = this->cap;
	if (this->cap - this->len < size)
	{
		cap += cap == 0 ? size : size * 2;
		tmp = this->str;
		if (!(this->str = ft_strnew(cap)))
			return (false);
		this->cap = cap;
		if (tmp)
			ft_strcpy(this->str, tmp);
		free(tmp);
	}
	return (true);
}

bool		concat_strings(t_string *this, char *str, size_t len)
{
	size_t			cap;
	char			*tmp;

	cap = this->cap;
	if (this->len + len < this->cap)
	{
		if (len > cap * 2)
			cap += len * 2;
		else
			cap *= 2;
		if (!(tmp = ft_strnew(this->cap)))
			return (false);
		ft_strcat(tmp, this->str);
		this->cap = cap;
		free(this->str);
		this->str = tmp;
	}
	ft_strncat(this->str, str, len);
	this->len += len;
	return (true);
}
