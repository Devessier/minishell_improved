/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 09:34:03 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/05 11:11:34 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <unistd.h>
#include <sys/syslimits.h>

static const t_shell_builtin	sh_builtins[] = {
	{ "exit", sh_builtin_exit },
	{ "env", sh_builtin_env },
	{ "setenv", sh_builtin_setenv },
	{ "unsetenv", sh_builtin_unsetenv },
	{ "echo", sh_builtin_echo },
	{ "cd", sh_builtin_cd },
	{ "which", sh_builtin_which },
	{ NULL, NULL },
};

ssize_t							is_builtin(char *name)
{
	size_t	i;

	i = 0;
	while (sh_builtins[i++].name != NULL)
		if (ft_strcmp(sh_builtins[i - 1].name, name) == 0)
			return (i - 1);
	return (-1);
}

t_lookup_result					locate_xfile(char *name, char *path_env_var, char path[PATH_MAX])
{
	size_t	name_len;
	char	*start;
	char	*end;

	if (!path_env_var)
		return (LK_NOT_FOUND);
	name_len = ft_strlen(name);
	start = path_env_var + 5;
	while (*start)
	{
		if ((end = ft_strchr(start, ':')) == NULL)
			end = start + ft_strlen(start);
		if (end - start + 1 + name_len > PATH_MAX)
			return (LK_PATH_TOO_LONG);
		ft_strncpy(path, start, end - start);
		ft_strcpy(path + (end - start), "/");
		ft_strcpy(path + (end - start + 1), name);
		if (access(path, X_OK) == 0)
			return (LK_FOUND);
		start = end + !!*end;
	}
	return (LK_NOT_FOUND);
}

t_lookup_result					lookup_path(t_string *name, t_env *env, char path[PATH_MAX])
{
	const t_string	*path_env_var = get_env(env, "PATH");
	const bool		contains_slash = path_env_var && ft_strchr(name->buff, '/') != NULL;

	if (contains_slash)
		return (access(ft_strcpy(path, name->buff), X_OK) == 0 ? LK_FOUND : LK_NOT_FOUND);
	else if (is_builtin(name->buff) != -1)
		return (LK_BUILTIN);
	if (!path_env_var)
		return (LK_NOT_FOUND);
	return (locate_xfile(name->buff, path_env_var->buff, path));
}

int								copy_args_env(char *buffer[ARG_MAX], char path[PATH_MAX], t_ast_node *command, t_env *env)
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
	buffer[i++] = NULL;
	return (argc);
}

int								exec_xfile(char path[PATH_MAX], t_ast_node *command, t_env *env, int *status)
{
	char	*buffer[ARG_MAX];
	int		argc;
	char	**envp;
	pid_t	child_pid;

	argc = copy_args_env(buffer, path, command, env);
	envp = &buffer[argc + 1];
	if ((child_pid = fork()) == 0 && execve(path, buffer, envp) == -1)
		ft_putf("minishell: error occured during child fork: %s\n", path);
	else if (child_pid < 0)
		ft_putf("minishell: error occured during child fork: %s\n", path);
	else
		waitpid(child_pid, status, 0);
	return (0);
}

int								exec_builtin(t_string *name, t_ast_node *command, t_env *env)
{
	size_t	i;

	i = 0;
	while (sh_builtins[i++].name != NULL)
		if (ft_strcmp(sh_builtins[i - 1].name, name->buff) == 0)
			return (sh_builtins[i - 1].fn(command->payload.command.args, command->payload.command.len, env));
	return (127);
}

int								sh_exec(t_string *string, t_env *env)
{
	char			path[PATH_MAX];
	const t_lexer	lexer = sh_lexer(string);
	t_ast_node		root;
	size_t			i;
	t_lookup_result	result;
	int				status;

	if (lexer.state != GLOBAL_SCOPE)
		return (ft_putf("minishell: syntax error: %s\n", string->buff), 126);
	root = sh_construct_ast(&lexer);
	i = 0;
	while (i++ < root.payload.root.len)
	{
		if ((result = lookup_path(&root.payload.root.commands[i - 1].payload.command.string, env, path)) == LK_NOT_FOUND)
			ft_putf("minishell: command not found: %s\n", root.payload.root.commands[i - 1].payload.command.string.buff);
		if (result == LK_PATH_TOO_LONG)
			ft_putf("minishell: path to file is too long: %s\n", root.payload.root.commands[i - 1].payload.command.string);
		if (result == LK_FOUND)
			exec_xfile(path, &root.payload.root.commands[i - 1], env, &status);
		else if (result == LK_BUILTIN)
			status = exec_builtin(&root.payload.root.commands[i - 1].payload.command.string, &root.payload.root.commands[i - 1], env);
		else
			status = result == LK_NOT_FOUND ? 127 : 126;
	}
	return (status);
}
