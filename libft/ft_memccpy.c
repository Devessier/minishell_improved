/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 16:31:18 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/08 23:16:05 by Devessier        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	void	*tmp;

	if ((tmp = ft_memchr(src, c, n)) != NULL)
		return (ft_memcpy(dst, src, tmp - src + 1) + (tmp - src + 1));
	ft_memcpy(dst, src, n);
	return (NULL);
}
