/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/08 16:54:20 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/08 17:43:12 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <sys/syslimits.h>

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

char			*sh_complete_command(char *start, size_t len, t_env *env)
{
	const t_string	user_path = get_env(env, "PATH");
	char			path[PATH_MAX];
	char			*filename;
	const char		*slash = ft_strchr(start, '/');

	if (slash == NULL)
		if (search_builtin(start, len, path) && (filename = ft_strdup(path)) != NULL)
			return (filename);
	filename = NULL;
	if (user_path.len == 0)
		; // search only for builtins
	// search for builtins
	// search for binary files
	return (filename);	
}

char			*sh_complete_filename(char *start, size_t len)
{
	(void)start, (void)len;
	return (NULL);
}
