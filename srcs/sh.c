/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 17:09:14 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/22 16:48:53 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <signal.h>
#include "sh.h"
#include "libft.h"
#include "readline.h"

static bool		exit_sh = false;
static t_env	*g_env = NULL;

static bool				sh_exit(char c, t_readline *rl, t_string *line)
{
	(void)rl;
	(void)line;
	if (c == 0x4 && line->len == 0)
		return (!(exit_sh = true));
	return (true);
}

static t_completion		autocomplete_state(t_string *line, t_readline *rl)
{
	t_completion			completion;
	size_t					i;
	char					previous;

	completion = (t_completion) { AC_COMMAND, 0, NULL };
	i = 0;
	previous = 0;
	while (i < rl->cursor)
	{
		if (line->buff[i] == ' ' && previous == ';')
		{
			i++;
			continue ;
		}
		if (line->buff[i] == ';')
			completion.state = AC_COMMAND;
		else if ((completion.state == AC_COMMAND && line->buff[i] == ' ')
				|| (!(line->buff[i] == '\'' || line->buff[i] == '\"') && completion.state != AC_COMMAND))
			completion.state = AC_FILE;
		previous = line->buff[i++];
	}
	while (i > 0 && !(line->buff[i - 1] == '\'' || line->buff[i - 1] == '\"' || line->buff[i - 1] == ' '))
		if (line->buff[--i] != '\0')
			completion.len++;
	completion.start = &line->buff[i];
	return (completion);
}

bool		sh_autocomplete(char c, t_readline *rl, t_string *line)
{
	char				path[PATH_MAX];
	const t_completion	completion = autocomplete_state(line, rl);
	char				*filename;
	size_t				filename_len;
	size_t				diff;

	if (c != '\t' || line->len == 0 || completion.len == 0)
		return (true);
	diff = completion.start - line->buff;
	filename = completion.state == AC_COMMAND
		? sh_complete_command(completion.start, completion.len, path, g_env)
		: sh_complete_filename(completion.start, completion.len, path, false);
	if (filename != NULL && ft_extend_string(line, (filename_len = ft_strlen(filename)))
			&& filename_len > completion.len)
	{	
		ft_memmove(line->buff + diff + filename_len, line->buff + diff + completion.len, line->len - (completion.len + diff));
		ft_memmove(line->buff + diff, filename, filename_len);
		line->len += filename_len - completion.len;
		ft_rl_move_cursor(rl, line, JUMP_TO_N_CHAR, filename_len - completion.len);
	}
	else
		ft_putchar(0x7);
	return (true);
}

static void	sighandler(int sig)
{
	if (sig == SIGINT)
	{
		kill(g_child_pid, SIGINT);
		ft_putchar('\n');
	}
}

static void	setup_sighandlers(void)
{
	signal(SIGQUIT, sighandler);
	signal(SIGINT, sighandler);
	signal(SIGTSTP, sighandler);
}

int			main(int argc, char **argv, char **envp)
{
	(void)argc, (void)argv;
	const int	stdin_tty = isatty(0);
	const t_env	env = copy_env(envp);
	int			status;
	t_string	line;

	ft_rl_config_termios(2);
	setup_sighandlers();
	ft_rl_bind_key(0x4, sh_exit);
	ft_rl_bind_key('\t', sh_autocomplete);
	status = 0;
	while (!exit_sh)
	{
		g_env = (t_env *)&env;
		ft_rl_init();
		line = ft_readline("Minishell Improved", status == 0 ? RL_BLUE : RL_RED);
		if (line.len > 0)
		{
			ft_rl_config_termios(0);
			if (ft_strncmp(line.buff, "exit", 4) == 0)
				exit_sh = true;
			else
				status = sh_exec(&line, (t_env *)&env);
		}
		else if (!stdin_tty)
			exit_sh = true;
		ft_free_string(&line);
	}
	if (g_must_print_prompt)
		ft_putf("exit\n");
	destroy_env((t_env *)&env);
	ft_rl_config_termios(0);
}
