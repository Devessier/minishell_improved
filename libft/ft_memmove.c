/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 08:57:00 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/14 14:16:57 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*tmp;
	char	*dest;

	if (src == dst)
		return (dst);
	if (src > dst)
		return (ft_memcpy(dst, src, len));
	tmp = (char *)src + len - 1;
	dest = (char *)dst + len - 1;
	while (tmp + 1 != (char *)src)
		*dest-- = *tmp--;
	return (dst);
}
