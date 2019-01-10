/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 10:58:05 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/14 11:02:58 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t		ft_lstsize(t_list *lst)
{
	t_list	*node;
	size_t	i;

	if (!lst)
		return (0);
	i = 1;
	node = lst;
	while ((node = node->next))
		i++;
	return (i);
}
