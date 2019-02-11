/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/08 16:54:20 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/11 16:53:46 by bdevessi         ###   ########.fr       */
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
	while (sh_builtins[i].fn != NULL)
		if (ft_strnstr(sh_builtins[i++].name, start, len) != NULL)
		{
			ft_strcpy(path, sh_builtins[i - 1].name);
			return (true);
		}
	return (false);
}

static bool		search_command(char *command, char path[PATH_MAX], char *user_path)
{
	char			path_to_command[PATH_MAX];
	DIR				*dir;
	size_t			max_len;
	char			*start;
	char			*end;
	struct dirent	*d;

	start = user_path;
	max_len = 0;
	while (start && *start)
	{
		if ((end = ft_strchr(start, ':')) == NULL)
			end = ft_strchr(start, '\0');
		ft_strncpy(path_to_command, start, end - start);
		path_to_command[end - start] = '\0';
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
		start = end + !!*end;
	}
	return (max_len != 0);
}

char			*sh_complete_command(char *start, size_t len, t_env *env)
{
	const t_string	user_path = get_env(env, "PATH");
	static char		path[PATH_MAX];
	const char		*slash = ft_strchr(start, '/');
	char			tmp;

	ft_bzero(path, PATH_MAX);
	if (slash == NULL)
		if (search_builtin(start, len, path))
			return (path);
	if (user_path.len == 0)
		return (NULL); // search only for builtins
	tmp = start[len];
	start[len] = '\0';
	if (search_command(start, path, user_path.buff))
	{
		start[len] = tmp;
		return (path);
	}
	// search for binary files
	return (NULL);
}

char			*sh_complete_filename(char *start, size_t len)
{
	(void)start, (void)len;
	return (NULL);
}
