/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stritercheck.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 13:35:24 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/05 13:37:42 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_stritercheck(char *str, int (*f)(int))
{
	int	status;

	status = 1;
	while (str && *str && status == 1)
		status = f(*str++);
	return (status);
}
