/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 09:45:43 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/22 14:48:46 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "readline.h"
#include <stdbool.h>
#include <sys/stat.h>

int		sh_builtin_echo(t_string *args, size_t len, t_env *env)
{
	const bool	endl = !(len > 0 && ft_strcmp(args[0].buff, "-n") == 0);
	size_t		i;

	(void)env;
	i = !endl;
	while (i++ < len)
		ft_putf("%s%c", args[i - 1].buff, i != len ? ' ' : '\0');
	if (endl)
		ft_putchar('\n');
	return (0);
}

int		change_directory(char *new, char *current, bool print_new_location, t_env *env)
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
	if (chdir(new) == -1)
	{
		ft_putf("minishell: cd: couldn't change the working directory: %s\n", new);
		return (127);
	}
	if (getcwd(new, PATH_MAX) == NULL)
	{
		ft_putf("minishell: cd: couldn't resolve the current working directory: %s\n", new);
		return (127);
	}
	if (print_new_location)
		ft_putendl(new);
	if (!put_env(env, "PWD", new))
	{
		ft_putf("minishell: cd: couldn't set the PWD variable: %s\n", new);
		return (127);
	}
	return (0);
}

int		sh_builtin_cd(t_string *args, size_t len, t_env *env)
{
	char		cwd_buff[PATH_MAX];
	char		path[PATH_MAX];
	t_string	string;
	bool		print_new_location;

	print_new_location = false;
	if (len == 0)
	{
		if ((string = get_env(env, "HOME")).len == 0)
			return (0);
		ft_strcpy(path, string.buff);
	}
	else if (*args[0].buff == '-' && args[0].buff[1] == '\0')
	{
		if ((string = get_env(env, "OLDPWD")).len == 0)
		{
			ft_putstr("minishell: cd: OLDPWD not set\n");
			return (127);
		}
		ft_strcpy(path, string.buff);
		print_new_location = true;
	}
	else
		ft_strcpy(path, args[0].buff);
	if (getcwd(cwd_buff, sizeof(cwd_buff)) == NULL)
	{
		ft_putf("minishell: cd: couldn't get the current working directory: %s\n", args[0].buff);
		return (127);
	}
	return (change_directory(path, cwd_buff, print_new_location, env));
}

int		sh_builtin_which(t_string *args, size_t len, t_env *env)
{
	char			path[PATH_MAX];
	t_lookup_result	result;
	size_t			i;

	if (len == 0)
		return (ft_putendl("minishell: which: too few arguments"), 127);
	i = 0;
	while (i++ < len)
		if ((result = sh_search_command(&args[i - 1], env, path)) == LK_NOT_FOUND)
			ft_putf("%s: command not found\n", args[i - 1].buff);
		else if (result == LK_BUILTIN)
			ft_putf("%s: shell built-in command\n", args[i - 1].buff);
		else if (result == LK_FOUND)
			ft_putendl(path);
		else if (result == LK_NO_RIGHTS)
			ft_putf("%s: permission denied\n", path);
		else
			ft_putf("%s: path to file too long\n", path);
	return (0);
}
