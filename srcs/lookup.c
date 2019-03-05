/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lookup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 11:47:18 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/05 11:56:29 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include <stdbool.h>
#include <sys/stat.h>

t_lookup_result	lookup_path(char *name, char *path_env_var,
	char path[PATH_MAX])
{
	struct stat	stats;
	size_t		name_len;
	char		*start;
	char		*end;

	if (!path_env_var)
		return (LK_NOT_FOUND);
	name_len = ft_strlen(name);
	start = path_env_var;
	while (*start)
	{
		if ((end = ft_strchr(start, ':')) == NULL)
			end = start + ft_strlen(start);
		if (end - start + 1 + name_len > PATH_MAX)
			return (LK_PATH_TOO_LONG);
		ft_strncpy(path, start, end - start);
		ft_strcpy(path + (end - start), "/");
		ft_strcpy(path + (end - start + 1), name);
		if (stat(path, &stats) == 0)
			return (stats.st_mode & S_IXUSR ? LK_FOUND : LK_NO_RIGHTS);
		start = end + !!*end;
	}
	return (LK_NOT_FOUND);
}

t_lookup_result	sh_search_command(t_string *name,
	t_env *env, char path[PATH_MAX])
{
	const t_string	path_env_var = get_env(env, "PATH", 4);
	const bool		contains_slash = ft_strchr(name->buff, '/') != NULL;
	struct stat		stats;

	if (contains_slash)
	{
		if (lstat(ft_strcpy(path, name->buff), &stats) != 0)
			return (LK_NOT_FOUND);
		return (((stats.st_mode & S_IFREG)
			&& (stats.st_mode & S_IXUSR)) ? LK_FOUND : LK_NO_RIGHTS);
	}
	else if (is_builtin(name->buff) != -1)
		return (LK_BUILTIN);
	if (!(path_env_var.len > 0))
		return (LK_NOT_FOUND);
	return (lookup_path(name->buff, path_env_var.buff, path));
}

int				copy_args_env(char *buffer[ARG_MAX],
	char path[PATH_MAX], t_ast_node *command, t_env *env)
{
	int		argc;
	size_t	i;
	size_t	j;

	i = 0;
	*buffer = path;
	while (i++ < command->payload.command.len)
		buffer[i] = command->payload.command.args[i - 1].buff;
	argc = i;
	buffer[i++] = NULL;
	j = 0;
	while (j < env->len)
		buffer[i++] = env->vars[j++].buff;
	buffer[i] = NULL;
	return (argc);
}
