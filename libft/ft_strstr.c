/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 15:38:09 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/14 11:40:08 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	char	*string;
	char	*substring;

	if (*needle == '\0')
		return ((char *)haystack);
	while (*haystack)
	{
		string = (char *)haystack;
		substring = (char *)needle;
		while (*haystack && *substring && *haystack++ == *substring)
			substring++;
		if (!*substring)
			return (string);
		haystack = string + 1;
	}
	return (NULL);
}
