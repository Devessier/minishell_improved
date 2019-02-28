/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 09:45:43 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/28 15:19:03 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "readline.h"

int			sh_builtin_echo(t_string *args, size_t len, t_env *env)
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

int			sh_builtin_which(t_string *args, size_t len, t_env *env)
{
	char			path[PATH_MAX];
	t_lookup_result	result;
	size_t			i;

	if (len == 0)
	{
		ft_putendl("minishell: which: too few arguments");
		return (127);
	}
	i = 0;
	while (i++ < len)
		if ((result = sh_search_command(&args[i - 1],
				env, path)) == LK_NOT_FOUND)
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
