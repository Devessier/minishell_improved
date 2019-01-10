/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 14:17:06 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/08 11:40:17 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t		ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t		i;

	i = 0;
	while (i[dst] && i < size)
		i++;
	if (size - i == 0)
		return (i + ft_strlen(src));
	while (*src && i < size - 1)
		(i++)[dst] = *src++;
	if (i < size)
		i[dst] = '\0';
	while (*src++)
		i++;
	return (i);
}
