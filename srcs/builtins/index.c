/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/02 12:54:28 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/02 13:24:59 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"

t_shell_builtin		g_sh_builtins[] = {
	{ "env", sh_builtin_env },
	{ "setenv", sh_builtin_setenv },
	{ "unsetenv", sh_builtin_unsetenv },
	{ "echo", sh_builtin_echo },
	{ "cd", sh_builtin_cd },
	{ "which", sh_builtin_which },
	{ "exit", NULL },
	{ NULL, NULL },
};


ssize_t				is_builtin(const char *name)
{
	size_t	i;

	i = 0;
	while (g_sh_builtins[i++].name != NULL)
		if (ft_strcmp(g_sh_builtins[i - 1].name, name) == 0)
			return (i - 1);
	return (-1);
}
