/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 16:01:02 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/03 19:01:54 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <stdbool.h>

t_env	copy_env(char **envp)
{
	t_env		env;
	size_t		cap;
	size_t		i;

	env = (t_env) { 0, 0, NULL };
	if (*envp == NULL || **envp == '\0')
		return (env);
	while (envp[env.len] != NULL)
		env.len++;
	cap = ft_round_up_2n_pow(env.len);
	if (!(env.vars = malloc(sizeof(t_string) * cap)))
		return ((t_env) { 0, 0, NULL });
	i = 0;
	while (i++ < env.len)
		env.vars[i - 1] = ft_new_string(envp[i - 1], false);
	env.cap = cap;
	return (env);
}

void	print_env(const t_env *env)
{
	size_t	i;

	i = 0;
	while (i < env->len)
		ft_putendl(env->vars[i++].buff);
}

bool	put_env(t_env *env, const char *name, const char *value)
{
	const size_t	value_len = ft_strlen(value);
	size_t			i;
	char			*equal;
	t_string		*tmp;

	if (!(name && *name))
		return (false);
	i = 0;
	while (i < env->len)
	{
		if (ft_strstr(env->vars[i].buff, name) == env->vars[i].buff)
			if ((equal = ft_strchr(env->vars[i].buff, '=')) != NULL
					&& (unsigned long)(equal - env->vars[i].buff) == ft_strlen(name))
			{
				if ((equal - env->vars[i].buff + 1 + value_len) > env->vars[i].cap)
				{
					if (!ft_extend_string(&env->vars[i], value_len + equal - env->vars[i].buff + 1 - env->cap))
						return (false);
					ft_strcpy(ft_strchr(env->vars[i].buff, '=') + 1, value);
				}
				else
					ft_strcpy(equal + 1, value);
				return (true);
			}
		i++;
	}
	if (!(env->len < env->cap))
	{
		if (!(tmp = malloc(sizeof(t_string) * (env->cap << 1))))
			return (false);
		i = 0;
		while (i++ < env->len)
			tmp[i - 1] = env->vars[i - 1];
		free(env->vars);
		env->vars = tmp;	
	}
	env->vars[env->len] = (t_string) { 0, 0, NULL };
	if (!ft_extend_string(&env->vars[env->len], ft_strlen(name) + 1 + value_len))
		return (false);
	ft_strcat(env->vars[env->len].buff, name);
	ft_strcat(env->vars[env->len].buff, "=");
	ft_strcat(env->vars[env->len++].buff, value);
	return (true);
}

bool	unset_env(t_env *env, const char *name)
{
	size_t	i;
	char	*equal;

	i = 0;
	while (i < env->len)
	{
		if (ft_strstr(env->vars[i].buff, name) == env->vars[i].buff)
			if ((equal = ft_strchr(env->vars[i].buff, '=')) != NULL
					&& (unsigned long)(equal - env->vars[i].buff) == ft_strlen(name))
			{
				ft_free_string(&env->vars[i]);
				while (i++ < env->len - 1)
					env->vars[i - 1] = env->vars[i];
				env->vars[--env->len] = (t_string) { 0, 0, NULL };
				return (true);
			}
		i++;
	}
	return (true);
}
