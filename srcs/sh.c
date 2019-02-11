/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 17:09:14 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/11 17:49:45 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "sh.h"
#include "libft.h"
#include <sys/ioctl.h>
#include "readline.h"

static bool		exit_sh = false;
static t_env	*g_env = NULL;

bool		sh_exit(char c, t_readline *rl, t_string *line)
{
	(void)rl;
	(void)line;
	if (c == 0x4 && line->len == 0)
		return (!(exit_sh = true));
	return (true);
}

t_completion		autocomplete_state(t_string *line, t_readline *rl)
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
	const t_completion	completion = autocomplete_state(line, rl);
	char				*filename;
	size_t				filename_len;
	size_t				diff;

	if (c != '\t' || line->len == 0 || completion.len == 0)
		return (true);
	if (completion.state == AC_COMMAND)
	{
		diff = completion.start - line->buff;
		if ((filename = sh_complete_command(completion.start, completion.len, g_env)) != NULL
			&& ft_extend_string(line, (filename_len = ft_strlen(filename)))
			&& filename_len > completion.len)
		{	
			ft_putf("diff = %d, result is %s, filename_len = %d, completion len = %d\n", diff, filename, filename_len, completion.len);
			ft_memmove(line->buff + diff + filename_len, line->buff + diff + completion.len, filename_len - completion.len);
			ft_putendl("after");
			ft_memmove(line->buff + diff + completion.len, filename + completion.len, filename_len - completion.len);
			line->len += filename_len - completion.len;
			ft_rl_move_cursor(rl, line, JUMP_TO_N_CHAR, filename_len - completion.len);
		}
		else
			ft_putchar(0x7);
	}
	return (true);
}

bool		sh_setup_rl_bound_functions(void)
{
	return (ft_rl_bind_key(0x4, sh_exit) && ft_rl_bind_key('\t', sh_autocomplete));
}

int			main(int argc, char **argv, char **envp)
{
	(void)argc, (void)argv;
	const t_env	env = copy_env(envp);
	int			status;
	t_string	line;

	status = 0;
	ft_rl_config_termios(2);
	sh_setup_rl_bound_functions();
	while (!exit_sh)
	{
		g_env = (t_env *)&env;
		ft_rl_init();
		line = ft_readline("Minishell Improved", status == 0 ? RL_BLUE : RL_RED);
		if (line.len > 0)
		{
			ft_rl_config_termios(0);
			status = sh_exec(&line, (t_env *)&env);
		}
	}
	ft_putf("exit\n");
	ft_rl_config_termios(0);
}
