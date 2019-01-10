/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 15:01:04 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/08 11:41:43 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*last;
	char	*tmp;

	if (c == '\0')
		return (ft_strchr(s, c));
	last = NULL;
	while ((tmp = ft_strchr(s, c)) != NULL)
	{
		last = tmp;
		s = tmp + 1;
	}
	return (last);
}
