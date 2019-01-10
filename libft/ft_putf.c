/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 15:33:42 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/14 14:17:31 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putf(const char *format, ...)
{
	va_list		args;

	va_start(args, format);
	ft_putf_va(1, format, args);
	va_end(args);
}
