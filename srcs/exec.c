/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 09:34:03 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/05 11:47:55 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <unistd.h>
#include <sys/syslimits.h>
#include <sys/stat.h>

pid_t							g_child_pid;

static int						exec_xfile(char path[PATH_MAX],
	t_ast_node *command, t_env *env, int *status)
{
	char	*buffer[ARG_MAX / sizeof(char*)];
	int		argc;
	char	**envp;

	ft_bzero(buffer, sizeof(buffer) / sizeof(*buffer));
	argc = copy_args_env(buffer, path, command, env);
	envp = &buffer[argc + 1];
	if ((g_child_pid = fork()) == 0 && execve(path, buffer, envp) == -1)
		ft_putf("minishell: error occured during child fork: %s\n", path);
	else if (g_child_pid < 0)
		ft_putf("minishell: error occured during child fork: %s\n", path);
	else
		waitpid(g_child_pid, status, 0);
	return (0);
}

static int						exec_builtin(t_string *name,
	t_ast_node *command, t_env *env)
{
	size_t	i;

	i = 0;
	while (g_sh_builtins[i++].name != NULL)
		if (ft_strcmp(g_sh_builtins[i - 1].name, name->buff) == 0)
		{
			return (g_sh_builtins[i - 1].fn(command->payload.command.args,
				command->payload.command.len, env));
		}
	return (127);
}

static void						exec_command(t_ast_node *command,
	int *status, t_env *env, char path[PATH_MAX])
{
	t_lookup_result	result;

	if ((result = sh_search_command(&command->payload.command.string,
			env, path)) == LK_NOT_FOUND)
	{
		ft_putf("minishell: command not found: %s\n",
			command->payload.command.string.buff);
		*status = 127;
	}
	else if (result == LK_NO_RIGHTS)
		ft_putf("minishell: permission denied: %s\n",
			command->payload.command.string.buff);
	else if (result == LK_PATH_TOO_LONG)
		ft_putf("minishell: path to file is too long: %s\n",
			command->payload.command.string);
	else if (result == LK_FOUND)
		exec_xfile(path, command, env, status);
	else if (result == LK_BUILTIN)
		*status = exec_builtin(&command->payload.command.string,
			command, env);
}

int								sh_exec(t_string *string, t_env *env)
{
	char			path[PATH_MAX];
	const t_lexer	lexer = sh_lexer(string, env);
	t_ast_node		root;
	int				status;
	size_t			i;

	if (lexer.len == 0
		|| (root = sh_construct_ast(&lexer)).payload.root.len == 0)
		return (1);
	if (lexer.state != GLOBAL_SCOPE)
	{
		ft_putf("minishell: syntax error: %s\n", string->buff);
		return (126);
	}
	i = 0;
	status = 0;
	while (i++ < root.payload.root.len)
		exec_command(&root.payload.root.commands[i - 1],
			&status, env, path);
	destroy_ast(root, &lexer);
	return (status);
}
