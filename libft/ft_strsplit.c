/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 09:56:17 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/14 12:48:05 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_split_strdup(char *str, char c)
{
	char	*tmp;
	int		n;

	n = 0;
	while (str[n] && str[n] != c)
		n++;
	if (!(tmp = (char *)malloc(sizeof(char) * (n + 1))))
		return (NULL);
	while (*str && *str != c)
		*tmp++ = *str++;
	*tmp = '\0';
	return (tmp - n);
}

static int	nb_words(char const *s, char c)
{
	int		wc;
	char	state;

	wc = 0;
	state = 0;
	while (*s)
	{
		if (*s == c)
			state = 0;
		else if (state == 0)
		{
			state ^= 1;
			wc++;
		}
		s++;
	}
	return (wc);
}

char		**ft_strsplit(char const *s, char c)
{
	int		len;
	char	**tab;

	if (!(s && *s))
		return (s ? (char **)ft_memalloc(sizeof(char **)) : NULL);
	len = nb_words(s, c);
	if (!(tab = (char **)malloc(sizeof(char *) * (len + 1))))
		return (NULL);
	tab[len] = 0;
	if (*s != c)
		if (!(*tab++ = ft_split_strdup((char *)s, c)))
			return (NULL);
	while (*++s)
		if (*(s - 1) == c && *s != c)
			if (!(*tab++ = ft_split_strdup((char *)s, c)))
				return (NULL);
	return (tab - len);
}
