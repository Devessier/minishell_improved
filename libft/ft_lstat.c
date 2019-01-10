/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstat.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 10:39:44 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/14 11:05:35 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list		*ft_lstat(t_list *lst, unsigned int pos)
{
	if (!pos)
		return (lst);
	while (--pos && lst && lst->next)
		lst = lst->next;
	return (pos ? NULL : lst->next);
}
