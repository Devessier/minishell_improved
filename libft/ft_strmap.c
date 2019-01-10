/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 11:32:59 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/12 17:01:14 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	size_t	i;
	size_t	len;
	char	*str;

	if (!(s && f))
		return (NULL);
	i = -1;
	len = ft_strlen(s);
	if (!(str = ft_strnew(len)))
		return (NULL);
	while (++i < len)
		str[i] = f(s[i]);
	return (str);
}
