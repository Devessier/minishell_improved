/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putf_va.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 09:20:04 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/14 14:31:06 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int		is_special_char(char c)
{
	return (c == 'c' || c == 'd' || c == 's' || c == '%');
}

void			ft_putf_va(int fd, const char *format, va_list args)
{
	while (*format)
	{
		if (*format == '%')
		{
			if (format[1] == 'c')
				ft_putchar_fd((char)va_arg(args, int), fd);
			else if (format[1] == 'd')
				ft_putnbr_fd(va_arg(args, int), fd);
			else if (format[1] == 's')
				ft_putstr_fd(va_arg(args, char *), fd);
			else
				ft_putchar_fd(*format, fd);
			format = format + (is_special_char(format[1]) ? 2 : 1);
		}
		else
			ft_putchar_fd(*format++, fd);
	}
}
