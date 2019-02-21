/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 09:34:03 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/21 14:16:27 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <unistd.h>
#include <sys/syslimits.h>
#include <sys/stat.h>

t_shell_builtin					g_sh_builtins[] = {
	{ "env", sh_builtin_env },
	{ "setenv", sh_builtin_setenv },
	{ "unsetenv", sh_builtin_unsetenv },
	{ "echo", sh_builtin_echo },
	{ "cd", sh_builtin_cd },
	{ "which", sh_builtin_which },
	{ "exit", NULL },
	{ NULL, NULL },
};

pid_t							g_child_pid;

ssize_t							is_builtin(char *name)
{
	size_t	i;

	i = 0;
	while (g_sh_builtins[i++].name != NULL)
		if (ft_strcmp(g_sh_builtins[i - 1].name, name) == 0)
			return (i - 1);
	return (-1);
}

t_lookup_result					lookup_path(char *name, char *path_env_var, char path[PATH_MAX])
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

t_lookup_result					sh_search_command(t_string *name, t_env *env, char path[PATH_MAX])
{
	const t_string	path_env_var = get_env(env, "PATH");
	const bool		contains_slash = ft_strchr(name->buff, '/') != NULL;
	struct stat		stats;

	if (contains_slash)
	{
		if (lstat(ft_strcpy(path, name->buff), &stats) != 0)
			return (LK_NOT_FOUND);
		return (((stats.st_mode & S_IFREG) && (stats.st_mode & S_IXUSR)) ? LK_FOUND : LK_NO_RIGHTS);
	}
	else if (is_builtin(name->buff) != -1)
		return (LK_BUILTIN);
	if (!(path_env_var.len > 0))
		return (LK_NOT_FOUND);
	return (lookup_path(name->buff, path_env_var.buff, path));
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
	buffer[i] = NULL;
	return (argc);
}

int								exec_xfile(char path[PATH_MAX], t_ast_node *command, t_env *env, int *status)
{
	char	*buffer[ARG_MAX / 8];
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

int								exec_builtin(t_string *name, t_ast_node *command, t_env *env)
{
	size_t	i;

	i = 0;
	while (g_sh_builtins[i++].name != NULL)
		if (ft_strcmp(g_sh_builtins[i - 1].name, name->buff) == 0)
			return (g_sh_builtins[i - 1].fn(command->payload.command.args, command->payload.command.len, env));
	return (127);
}

int								sh_exec(t_string *string, t_env *env)
{
	char			path[PATH_MAX];
	const t_lexer	lexer = sh_lexer(string, env);
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
		if ((result = sh_search_command(&root.payload.root.commands[i - 1].payload.command.string, env, path)) == LK_NOT_FOUND)
			ft_putf("minishell: command not found: %s\n", root.payload.root.commands[i - 1].payload.command.string.buff);
		else if (result == LK_NO_RIGHTS)
			ft_putf("minishell: permission denied: %s\n", root.payload.root.commands[i - 1].payload.command.string.buff);
		else if (result == LK_PATH_TOO_LONG)
			ft_putf("minishell: path to file is too long: %s\n", root.payload.root.commands[i - 1].payload.command.string);
		else if (result == LK_FOUND)
			exec_xfile(path, &root.payload.root.commands[i - 1], env, &status);
		else if (result == LK_BUILTIN)
			status = exec_builtin(&root.payload.root.commands[i - 1].payload.command.string, &root.payload.root.commands[i - 1], env);
		else
			status = result == LK_NOT_FOUND ? 127 : 126;
	}
	destroy_ast(root, &lexer);
	return (status);
}
