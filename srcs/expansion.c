/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 16:14:37 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/28 18:44:05 by bdevessi         ###   ########.fr       */
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
		if (!((home = get_env(env, "HOME")).len > 0))
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

static bool					check_token_too_long(t_string *token, size_t j, size_t i, size_t max)
{
	if (i < max)
		return (false);
	ft_putf("mooove\n");
	ft_putf("i = %d, j = %d\n, len = %d", i, j, token->len);
	while (i < token->len && (ft_isalnum(token->buff[i]) || token->buff[i] == '_'))
	{
		i++;
		j++;
	}
	ft_putf("i = %d, j = %d\n", i, j);
	ft_memmove(token->buff + i - j - 1, token->buff + i, token->len - i);
	token->len -= j + 1;
	i -= j + 1;
	token->buff[token->len] = '\0';
	return (true);
}

static bool					copy_env_var(t_string *token, size_t *i, t_env *env)
{
	char		env_name[4096];
	size_t		j;
	t_string	dollar;

	j = 0;
	while (*i < token->len && *i < sizeof(env_name) && (ft_isalnum(token->buff[*i]) || token->buff[*i] == '_'))
		env_name[j++] = token->buff[(*i)++];
	if (!check_token_too_long(token, j, *i, sizeof(env_name)))
		return (true);
	env_name[j] = '\0';
	if (*env_name != '\0' && (dollar = get_env(env, env_name)).len > 0)
	{
		if (!ft_extend_string(token, dollar.len - j - 1))
			return (false);
		ft_memmove(token->buff + *i - j + dollar.len - 1, token->buff + *i, token->len - *i + 1);
		ft_memmove(token->buff + *i - j - 1, dollar.buff, dollar.len);
		token->len += dollar.len - j - 1;
	}
	else
	{
		ft_memmove(token->buff + *i - j - 1, token->buff + *i, token->len - *i);
		token->len -= j + 1;
		*i -= j + 1;
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
