/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 12:10:50 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/12 13:57:32 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*tmp;
	t_list	*first;
	t_list	*last;
	t_list	*data;

	if (!(lst && f))
		return (NULL);
	last = NULL;
	first = NULL;
	while (lst)
	{
		data = f(lst);
		if (!(tmp = ft_lstnew(data->content, data->content_size)))
			return (NULL);
		if (first == NULL)
			first = tmp;
		if (last != NULL)
			last->next = tmp;
		last = tmp;
		lst = lst->next;
	}
	last->next = NULL;
	return (first);
}
