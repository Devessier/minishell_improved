/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 09:57:50 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/21 11:12:40 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int n)
{
	char	*str;
	long	pow;
	int		len;
	char	sign;

	pow = 1;
	len = 1;
	sign = n < 0 ? -1 : 1;
	while (n / (pow *= 10))
		len++;
	pow /= 10;
	if (!(str = (char *)malloc(sizeof(char) * (len + (n < 0) + 1))))
		return (NULL);
	if (n < 0)
		*str++ = '-';
	while (pow)
	{
		*str++ = (n / pow % 10) * sign + '0';
		pow /= 10;
	}
	*str = '\0';
	return (str - (len + (n < 0)));
}

char	*ft_itoa_buff(int n, char *buffer)
{
	long		pow;
	int			len;
	char		sign;

	pow = 1;
	len = 1;
	sign = n < 0 ? -1 : 1;
	while (n / (pow *= 10))
		len++;
	pow /= 10;
	if (n < 0)
		*buffer++ = '-';
	while (pow)
	{
		*buffer++ = (n / pow % 10) * sign + '0';
		pow /= 10;
	}
	*buffer = '\0';
	return (buffer - (len + (n < 0)));
}
