/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 16:01:02 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/01 17:45:16 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"

char	*copy_env(char **envp)
{
	size_t	strings_count;
	size_t	len;
	char	*env;

	if (*envp == NULL || **envp == '\0')
		return (NULL);
	len = 0;
	while (envp[strings_count] != NULL)
		len += ft_strlen(envp[strings_count++]);
	len += strings_count + 1;
	if (!(env = malloc(sizeof(char) * (len + 1))))
}
