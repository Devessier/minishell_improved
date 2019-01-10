/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 11:04:35 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/13 15:02:30 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	int		pow;
	int		tmp;
	char	sign;

	pow = 1;
	sign = n < 0 ? -1 : 1;
	tmp = n;
	while (tmp /= 10)
		pow *= 10;
	if (sign == -1)
		ft_putchar_fd('-', fd);
	while (pow)
	{
		ft_putchar_fd(n / pow % 10 * sign + '0', fd);
		pow /= 10;
	}
}
