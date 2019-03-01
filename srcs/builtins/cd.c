/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 11:44:24 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/01 11:17:54 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include <stdbool.h>
#include <sys/stat.h>

static int		change_directory2(char *new, t_env *env)
{
	if (chdir(new) == -1)
	{
		ft_putf("minishell: cd: couldn't change"
				" the working directory: %s\n", new);
		return (127);
	}
	if (getcwd(new, PATH_MAX) == NULL)
	{
		ft_putf("minishell: cd: couldn't resolve"
				" the current working directory: %s\n", new);
		return (127);
	}
	if (!put_env(env, "PWD", new))
	{
		ft_putf("minishell: cd: couldn't"
				" set the PWD variable: %s\n", new);
		return (127);
	}
	return (0);
}

static int		change_directory(char *new, char *current, t_env *env)
{
	struct stat	stats;

	if (lstat(new, &stats) == -1)
	{
		ft_putf("minishell: cd: no such file or directory: %s\n", new);
		return (127);
	}
	if (!(stats.st_mode & S_IFDIR))
	{
		ft_putf("minishell: cd: not a directory: %s\n", new);
		return (127);
	}
	if (access(new, X_OK) == -1)
	{
		ft_putf("minishell: cd: permission denied: %s\n", new);
		return (127);
	}
	if (!put_env(env, "OLDPWD", current))
	{
		ft_putf("minishell: cd: couldn't set the OLWPWD variable: %s\n", new);
		return (127);
	}
	return (change_directory2(new, env));
}

static int		cd_hyphen(t_env *env, t_string *string)
{
	if ((*string = get_env(env, "OLDPWD", 6)).len == 0)
	{
		ft_putstr("minishell: cd: OLDPWD not set\n");
		return (127);
	}
	return (0);
}

int				sh_builtin_cd(t_string *args, size_t len, t_env *env)
{
	char			cwd_buff[PATH_MAX];
	char			path[PATH_MAX];
	t_string		string;
	char			*src;

	src = NULL;
	if (len == 0 && (string = get_env(env, "HOME", 4)).len == 0)
		return (0);
	else if (len > 0 && *args[0].buff == '-' && args[0].buff[1] == '\0')
	{
		if (cd_hyphen(env, &string) != 0)
			return (127);
	}
	else if (len > 0)
		src = args[0].buff;
	ft_strcpy(path, src == NULL ? string.buff : src);
	if (getcwd(cwd_buff, sizeof(cwd_buff)) == NULL)
	{
		ft_putf("minishell: cd: couldn't get"
				" the current working directory: %s\n", args[0].buff);
		return (127);
	}
	return (change_directory(path, cwd_buff, env));
}
