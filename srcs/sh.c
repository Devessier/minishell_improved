/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 17:09:14 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/11 13:06:05 by bdevessi         ###   ########.fr       */
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

bool		sh_autocomplete(char c, t_readline *rl, t_string *line)
{
	(void)rl;
	size_t	cmd_len;
	char	*filename;

	if (c != '\t' || line->len == 0)
		return (true);
	cmd_len = 0;
	while (cmd_len < line->len && !(line->buff[cmd_len] == ' ' || line->buff[cmd_len] == ';'))
		cmd_len++;
	if (rl->cursor < cmd_len)
		return (true);
	filename = sh_complete_command(line->buff, cmd_len, g_env);
	if (filename != NULL && ft_extend_string(line, ft_strlen(filename)))
		ft_putf("filename = %s\n", filename);
	else
		ft_putchar(0x7);
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
