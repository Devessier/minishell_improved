/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 17:42:57 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/01 18:49:50 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	overwrite_env(t_env *env, const char *name,
	const char *value, size_t value_len)
{
	char	*equal;
	size_t	i;

	i = 0;
	while (i < env->len)
	{
		if (ft_strstr(env->vars[i].buff, name) == env->vars[i].buff
				&& (equal = ft_strchr(env->vars[i].buff, '=')) != NULL
				&& (equal - env->vars[i].buff) == (long)ft_strlen(name))
		{
			if ((equal - env->vars[i].buff + 1 + value_len) > env->vars[i].cap)
			{
				if (!ft_extend_string(&env->vars[i], value_len))
					return (-1);
				ft_strcpy(ft_strchr(env->vars[i].buff, '=') + 1, value);
			}
			else
				ft_strcpy(equal + 1, value);
			env->vars[i].len = value_len + ft_strlen(name) + 1;
			return (0);
		}
		i++;
	}
	return (1);
}

static bool	extend_env(t_env *env)
{
	size_t		cap;
	t_string	*tmp;
	size_t		i;

	if (env->len < env->cap)
		return (true);
	cap = env->cap == 0 ? 1 : env->cap;
	if (!(tmp = malloc(sizeof(t_string) * (cap << 1))))
		return (false);
	i = 0;
	while (i++ < env->len)
		tmp[i - 1] = env->vars[i - 1];
	free(env->vars);
	env->vars = tmp;
	env->cap = cap << 1;
	return (true);
}

bool		put_env(t_env *env, const char *name, const char *value)
{
	const size_t	name_len = ft_strlen(name);
	const size_t	value_len = ft_strlen(value);
	int				result;

	if (!(name && *name)
			|| (result = overwrite_env(env, name, value, value_len)) == -1
			|| !extend_env(env))
		return (false);
	else if (result == 0)
		return (true);
	env->vars[env->len] = (t_string) {
		.len = name_len + 1 + value_len,
		.cap = 0,
		.buff = NULL
	};
	if (!ft_extend_string(&env->vars[env->len], env->vars[env->len].len))
		return (false);
	ft_strcpy(env->vars[env->len].buff, name);
	ft_strcat(env->vars[env->len].buff, "=");
	ft_strcat(env->vars[env->len++].buff, value);
	return (true);
}

bool		unset_env(t_env *env, const char *name)
{
	size_t	i;
	char	*equal;

	i = 0;
	while (i < env->len)
	{
		if (ft_strstr(env->vars[i].buff, name) == env->vars[i].buff
				&& (equal = ft_strchr(env->vars[i].buff, '=')) != NULL
				&& (equal - env->vars[i].buff) == (long)ft_strlen(name))
		{
			free(env->vars[i].buff);
			while (i++ < env->len - 1)
				env->vars[i - 1] = env->vars[i];
			env->vars[--env->len] = (t_string) { 0, 0, NULL };
			break ;
		}
		i++;
	}
	return (true);
}
