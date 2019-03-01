/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/08 16:54:20 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/01 11:17:30 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <sys/syslimits.h>
#include <dirent.h>
#include <sys/stat.h>

static bool		search_builtin(char *start, size_t len, char path[PATH_MAX])
{
	size_t	i;

	i = 0;
	while (g_sh_builtins[i].fn != NULL)
		if (ft_strnstr(g_sh_builtins[i++].name, start, len) != NULL)
		{
			ft_strcpy(path, g_sh_builtins[i - 1].name);
			return (true);
		}
	return (false);
}

static bool		search_command(char *command,
	char path[PATH_MAX], char *start_end[2])
{
	char			path_to_command[PATH_MAX];
	DIR				*dir;
	size_t			max_len;
	struct dirent	*d;

	max_len = 0;
	while (*start_end && **start_end)
	{
		if ((start_end[1] = ft_strchr(*start_end, ':')) == NULL)
			start_end[1] = ft_strchr(*start_end, '\0');
		ft_strncpy(path_to_command, *start_end, start_end[1] - start_end[0]);
		path_to_command[start_end[1] - start_end[0]] = '\0';
		dir = opendir(path_to_command);
		while (dir && (d = readdir(dir)) != NULL)
			if ((max_len == 0 || d->d_namlen < max_len)
				&& ft_strstr(d->d_name, command) == d->d_name)
			{
				ft_strcpy(path, d->d_name);
				max_len = d->d_namlen;
			}
		if (dir)
			closedir(dir);
		start_end[0] = start_end[1] + !!*start_end[1];
	}
	return (max_len != 0);
}

static char		*search_file(char *string,
	char path[PATH_MAX], bool must_exec)
{
	char			tmp[PATH_MAX];
	DIR				*dir;
	struct dirent	*d;
	size_t			max_len;

	if (!(dir = opendir(path)))
		return (NULL);
	max_len = 0;
	while ((d = readdir(dir)) != NULL)
		if ((max_len == 0 || d->d_namlen <= max_len)
			&& !(ft_strnstr(d->d_name, ".", 1) != NULL
				|| ft_strnstr(d->d_name, "..", 2) != NULL)
			&& ft_strstr(d->d_name, string) == d->d_name)
		{
			ft_strcpy(tmp, path);
			if (!(*tmp == '/' && tmp[1] == '\0'))
				ft_strcat(tmp, "/");
			ft_strcat(tmp, d->d_name);
			if ((must_exec && access(tmp, X_OK) == 0)
				|| !must_exec)
				max_len = d->d_namlen;
		}
	ft_strcpy(path, tmp);
	closedir(dir);
	return (max_len == 0 ? NULL : path);
}

char			*sh_complete_filename(char *start, size_t len,
	char path[PATH_MAX], bool must_exec)
{
	char			*slash;
	char			*result;
	char			c;

	slash = ft_strrchr(start, '/');
	c = start[len];
	start[len] = '\0';
	if (slash == NULL)
		ft_strcpy(path, ".");
	else
	{
		*slash = '\0';
		ft_strcpy(path, *start == '\0' ? "/" : start);
	}
	result = search_file(slash == NULL ? start : slash + 1, path, must_exec);
	start[len] = c;
	if (slash != NULL)
		*slash = '/';
	return (result);
}

char			*sh_complete_command(char *start, size_t len,
	char path[PATH_MAX], t_env *env)
{
	const t_string	user_path = get_env(env, "PATH", 4);
	const char		*slash = ft_strchr(start, '/');
	char			tmp;
	char			*start_end[2];

	ft_bzero(path, PATH_MAX);
	if (slash == NULL && search_builtin(start, len, path))
		return (path);
	else if (slash != NULL)
		return (sh_complete_filename(start, len, path, true));
	if (user_path.len == 0)
		return (NULL);
	tmp = start[len];
	start[len] = '\0';
	start_end[0] = user_path.buff;
	if (search_command(start, path, start_end))
	{
		start[len] = tmp;
		return (path);
	}
	start[len] = tmp;
	return (NULL);
}
