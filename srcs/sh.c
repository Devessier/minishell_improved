/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 17:09:14 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/28 15:15:42 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "sh.h"
#include "libft.h"
#include "readline.h"

bool		g_exit_sh = false;
t_env		*g_env = NULL;

static bool	sh_exit(char c, t_readline *rl, t_string *line)
{
	(void)rl;
	(void)line;
	if (c == 0x4 && line->len == 0)
		return (!(g_exit_sh = true));
	return (true);
}

void		sighandler(int sig)
{
	if (sig == SIGINT)
	{
		kill(g_child_pid, SIGINT);
		ft_putchar('\n');
	}
}

static void	init_shell(void)
{
	ft_rl_config_termios(2);
	signal(SIGQUIT, sighandler);
	signal(SIGINT, sighandler);
	signal(SIGTSTP, sighandler);
	ft_rl_bind_key(0x4, sh_exit);
	ft_rl_bind_key('\t', sh_autocomplete);
}

static void	shell_loop(t_env env)
{
	const int	stdin_tty = isatty(0);
	int			status;
	t_string	line;
	size_t		prompt_len;

	status = 0;
	while (!g_exit_sh)
	{
		g_env = &env;
		ft_rl_init();
		line = ft_readline(sh_prompt(&prompt_len), prompt_len,
			status == 0 ? RL_BLUE : RL_RED);
		if (line.len > 0)
		{
			ft_rl_config_termios(0);
			if (ft_strncmp(line.buff, "exit", 4) == 0)
				g_exit_sh = true;
			else
				status = sh_exec(&line, &env);
		}
		else if (!stdin_tty)
			g_exit_sh = true;
		ft_free_string(&line);
	}
}

int			main(int argc, char **argv, char **envp)
{
	t_env	env;

	(void)argc;
	(void)argv;
	env = copy_env(envp);
	init_shell();
	shell_loop(env);
	if (g_must_print_prompt)
		ft_putf("exit\n");
	destroy_env((t_env *)&env);
	ft_rl_config_termios(0);
}
