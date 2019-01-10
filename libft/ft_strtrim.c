/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 13:15:02 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/14 14:33:59 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_space(char c)
{
	return (c == ' ' || c == '\n' || c == '\t');
}

char		*ft_strtrim(char const *s)
{
	size_t	len;

	if (!s)
		return (NULL);
	while (*s && is_space(*s))
		s++;
	len = ft_strlen(s);
	while (len && is_space(s[len - 1]))
		len--;
	return (ft_strsub(s, 0, len));
}
