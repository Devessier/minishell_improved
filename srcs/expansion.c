/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 16:14:37 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/01 13:27:31 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include <pwd.h>

char				g_shell_pid[10] = { 0 };
size_t				g_shell_pid_len = 0;

bool						expand_tildes(t_string *token, t_env *env)
{
	char			home_dir[PATH_MAX];
	t_string		home;
	struct passwd	*passwd;
	size_t			i;
	char			*str;

	str = token->buff + 1;
	i = 0;
	while (*str != '\0' && *str != '/')
		home_dir[i++] = *str++;
	home_dir[i] = '\0';
	if (*home_dir == '\0')
	{
		if (!((home = get_env(env, "HOME", 4)).len > 0))
		{

			if (!(passwd = getpwnam(getlogin())) || passwd->pw_dir == NULL)
				return (false);
			ft_strcpy(home_dir, passwd->pw_dir);
		}
		else
			ft_strcpy(home_dir, home.buff);
	}
	else if ((passwd = getpwnam(home_dir)) != NULL && passwd->pw_dir != NULL)
		ft_strcpy(home_dir, passwd->pw_dir);
	else
		return (false);
	ft_strcat(home_dir, str);
	if (!ft_extend_string(token, ft_strlen(home_dir)))
		return (false);
	ft_strcpy(token->buff, home_dir);
	return (true);
}

static bool					copy_shell_pid(t_string *token, size_t i)
{
	if (g_shell_pid_len > 2 && !ft_extend_string(token, g_shell_pid_len - 2))
		return (false);
	ft_memmove(token->buff + i + g_shell_pid_len - 1, token->buff + i + 1, token->len - i - 1);
	ft_memmove(token->buff + i - 1, g_shell_pid, g_shell_pid_len);
	token->len += g_shell_pid_len - 2;
	return (true);
}

static bool					copy_env_var(t_string *token, size_t *i, t_env *env)
{
	const size_t	start = *i;
	t_string		dollar;

	while (*i < token->len && (ft_isalnum(token->buff[*i]) || token->buff[*i] == '_'))
		(*i)++;
	if (start != *i && (dollar = get_env(env, token->buff + start, *i - start)).len > 0)
	{
		if (!ft_extend_string(token, dollar.len))
			return (false);
		ft_memmove(token->buff + start - 1 + dollar.len, token->buff + *i, token->len - *i);
		ft_memmove(token->buff + start - 1, dollar.buff, dollar.len);
		token->len = start + dollar.len + (token->len - *i);
		*i = start - 1 + dollar.len;
	}
	else
	{
		ft_memmove(token->buff + start - 1, token->buff + *i, token->len - *i + 1);
		token->len -= (*i - start);
		*i = start - 1;
		token->buff[token->len] = '\0';
	}
	return (true);
}

bool						expand_dollars(t_string *token, t_env *env)
{
	size_t		i;

	i = 0;
	while (i < token->len && token->buff[i])
	{
		if (token->buff[i] == '$')
		{
			if (token->buff[++i] == '$' && !copy_shell_pid(token, i))
				return (false);
			else if (token->buff[i] == '_' || ft_isalpha(token->buff[i]))
			{
				if (!copy_env_var(token, &i, env))
					return (false);
				else
					continue ;
			}
		}
		i++;
	}
	return (true);
}
