/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 11:23:09 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/14 14:15:58 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list		*ft_lstnew(void const *content, size_t content_size)
{
	t_list	*lst;
	void	*lst_content;

	if (!(lst = (t_list *)malloc(sizeof(t_list))))
		return (NULL);
	lst_content = NULL;
	if (content != NULL)
	{
		if (!(lst_content = ft_memalloc(content_size)))
			return (NULL);
		ft_memcpy(lst_content, content, content_size);
	}
	lst->content = content ? lst_content : NULL;
	lst->content_size = content ? content_size : 0;
	lst->next = NULL;
	return (lst);
}
