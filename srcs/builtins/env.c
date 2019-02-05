/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/03 21:03:18 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/05 13:53:18 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <stdbool.h>

int		sh_builtin_env(t_string *args, size_t len, t_env *env)
{
	(void)args, (void)len;
	print_env(env);
	return (0);
}

int		variable_name_check(int c)
{
	return (ft_isalnum(c) || c == '_');
}

int		sh_builtin_setenv(t_string *args, size_t len, t_env *env)
{
	if (!args || len == 0)
		return (sh_builtin_env(args, len, env));
	if (len > 2 || !(ft_isalpha(*args[0].buff) || *args[0].buff == '_') || !ft_stritercheck(args[0].buff, variable_name_check))
	{
		if (!(ft_isalpha(*args[0].buff) || *args[0].buff == '_'))
			ft_putstr("minishell: setenv: variable name must begin with a letter or an underscore\n");
		else
			ft_putstr(len > 2 ? "minishell: setenv: too many arguments" : "minishell: setenv: variable name must only contain alphanumeric characters" "\n");
		return (1);
	}
	put_env(env, args[0].buff, len == 2 ? args[1].buff : "");
	return (0);
}

int		sh_builtin_unsetenv(t_string *args, size_t len, t_env *env)
{
	size_t	i;

	i = 0;
	while (i < len)
		unset_env(env, args[i++].buff);
	return (0);
}
