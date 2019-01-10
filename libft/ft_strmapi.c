/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 12:45:30 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/12 17:01:51 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
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
		str[i] = f(i, s[i]);
	return (str);
}
