/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 12:56:16 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/14 12:20:59 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*tmp;
	size_t	i;

	if (!s || !(tmp = (char *)malloc(len + 1)))
		return (NULL);
	i = -1;
	while (++i < len)
		i[tmp] = s[start + i];
	i[tmp] = '\0';
	return (tmp);
}
