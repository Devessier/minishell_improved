/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 16:01:02 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/01 18:54:40 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <stdbool.h>

t_env		copy_env(char **envp)
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

void		print_env(const t_env *env)
{
	size_t	i;

	i = 0;
	while (i < env->len)
		ft_putendl(env->vars[i++].buff);
}

t_string	get_env(t_env *env, const char *name, size_t len)
{
	size_t		i;
	char		*equal;

	i = 0;
	while (i++ < env->len)
	{
		if (ft_strncmp(env->vars[i - 1].buff, name, len) == 0
				&& (equal = ft_strchr(env->vars[i - 1].buff, '=')) != NULL
				&& (equal - env->vars[i - 1].buff) == (long)len)
			return ((t_string) {
				.len = env->vars[i - 1].len
					- (equal - env->vars[i - 1].buff) - 1,
				.cap = env->vars[i - 1].cap,
				.buff = equal + 1,
			});
	}
	return ((t_string) { 0, 0, NULL });
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
