/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 16:01:02 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/21 14:29:39 by bdevessi         ###   ########.fr       */
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

t_string	get_env(t_env *env, const char *name)
{
	size_t		i;
	char		*equal;
	t_string	value;

	i = 0;
	while (i++ < env->len)
		if (ft_strstr(env->vars[i - 1].buff, name) == env->vars[i - 1].buff
			&& (equal = ft_strchr(env->vars[i - 1].buff, '=')) != NULL
			&& (equal - env->vars[i - 1].buff) == (long)ft_strlen(name))
		{
			value = (t_string) {
				.len = env->vars[i - 1].len - (equal - env->vars[i - 1].buff) - 1,
				.cap = env->vars[i - 1].cap,
				.buff = equal + 1,
			};
			return (value);
		}
	return ((t_string) { 0, 0, NULL });
}

bool	put_env(t_env *env, const char *name, const char *value)
{
	const size_t	value_len = ft_strlen(value);
	size_t			i;
	char			*equal;
	t_string		*tmp;
	size_t			cap;

	if (!(name && *name))
		return (false);
	i = 0;
	while (i < env->len)
	{
		if (ft_strstr(env->vars[i].buff, name) == env->vars[i].buff)
			if ((equal = ft_strchr(env->vars[i].buff, '=')) != NULL
					&& (equal - env->vars[i].buff) == (long)ft_strlen(name))
			{
				if ((equal - env->vars[i].buff + 1 + value_len) > env->vars[i].cap)
				{
					if (!ft_extend_string(&env->vars[i], value_len))
						return (false);
					ft_strcpy(ft_strchr(env->vars[i].buff, '=') + 1, value);
				}
				else
					ft_strcpy(equal + 1, value);
				env->vars[i].len = value_len + ft_strlen(name) + 1;
				return (true);
			}
		i++;
	}
	if (!(env->len < env->cap))
	{
		cap = env->cap == 0 ? 1 : env->cap;
		if (!(tmp = malloc(sizeof(t_string) * (cap << 1))))
			return (false);
		i = 0;
		while (i++ < env->len)
			tmp[i - 1] = env->vars[i - 1];
		free(env->vars);
		env->vars = tmp;	
		env->cap = cap << 1;
	}
	env->vars[env->len] = (t_string) { ft_strlen(name) + 1 + value_len, 0, NULL };
	if (!ft_extend_string(&env->vars[env->len], env->vars[env->len].len))
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

bool		destroy_env(t_env *env)
{
	size_t	i;

	i = 0;
	while (i < env->len)
		ft_free_string(&env->vars[i++]);
	free(env->vars);
	return (true);
}
