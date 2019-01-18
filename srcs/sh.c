/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 17:09:14 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/18 15:50:31 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "sh.h"
#include "libft.h"
#include <sys/ioctl.h>

struct termios	g_original_termios;
t_sh			g_shell;

void	sh_reset_raw_mode(void)
{
	tcsetattr(0, TCSAFLUSH, &g_original_termios);
}

void	sh_setup_raw_mode(void)
{
	struct termios	raw_termios;

	tcgetattr(0, &g_original_termios);
	raw_termios = g_original_termios;
	raw_termios.c_iflag &= ~(ICRNL | IXON);
	raw_termios.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw_termios.c_cc[VMIN] = 0;
	raw_termios.c_cc[VTIME] = 1;
	tcsetattr(0, TCSAFLUSH, &raw_termios);
	atexit(sh_reset_raw_mode);
}

void	move_cursor(t_command *cmd, t_sh *sh, ssize_t move)
{
	const unsigned short	x_move = (sh->cursor.x + move) % sh->ws.ws_col;
	const unsigned short	y_move = (sh->cursor.y + move) / sh->ws.ws_col;
	const int				cases_before_cursor = sh->ws.ws_col * sh->cursor.y + sh->cursor.x - sh->name_len - 4;
	const int				induced_cases_by_move = y_move * sh->ws.ws_col + x_move - sh->name_len - 4;

	if (move == -10000)
		move_cursor(cmd, sh, cases_before_cursor);
	else if (move == 10000)
		move_cursor(cmd, sh, cmd->string.len - cases_before_cursor - 1);
	else if ((move < 0 && cases_before_cursor + induced_cases_by_move > 0)
			|| (move > 0 && cmd->string.len - cases_before_cursor - induced_cases_by_move > 0))
	{
		sh->cursor.x += x_move;
		sh->cursor.y += y_move;
	}
}

void	sh_cli_handlers(t_sh *shell, t_reader *reader, t_command *cmd, char c, char prev)
{
	char	cnext;

	if (c == 0x4 && cmd->string.len == 0)
	{
		ft_putf("exit\n");
		exit(0);
	}
	else if (c == 0x1b)
	{
		sh_getchar(reader, STDIN_FILENO, &cnext);
		if (cnext == '[')
		{
			sh_getchar(reader, STDIN_FILENO, &cnext);
			if (cnext == 'C' || cnext == 'D')
				move_cursor(cmd, shell, cnext == 'C' ? 1 : -1);
		}
	}
	else if (c == 0x7f || (c == 0x4 && shell->cursor.x < cmd->string.len))
		sh_command_del(cmd, shell, c == 0x7f);
	else if (c == 0x5 || c == 0x1)
		move_cursor(cmd, shell, c == 0x1 ? -10000 : 10000);
	else if (ft_isprint(c) && ((c == ' ' && prev != ' ') || c != ' '))
	{
		sh_command_cat(cmd, shell, c);
		move_cursor(cmd, shell, -1);
	}
	else if (c == 0x4)
		ft_putchar(0x7);
}

void	update_window_size(int sig)
{
	if (sig != SIGWINCH)
		return ;
	ft_putf("update\n");
	ioctl(STDIN_FILENO, TIOCGWINSZ, &g_shell.ws);
}

void	sh_init(t_sh *sh, const char *name)
{
	*sh = (t_sh) {
		.cursor = { 0, 0 },
		.primary_color = BLUE,
	};
	ioctl(STDIN_FILENO, TIOCGWINSZ, &g_shell.ws);
	ft_putf("col = %d\n", g_shell.ws.ws_col);
	signal(SIGWINCH, update_window_size);
	ft_strcpy(sh->name, name);
	sh->name_len = ft_strlen(sh->name);
	sh_setup_raw_mode();
}

int		main(int argc, char **argv, char **envp)
{
	(void)argc, (void)argv, (void)envp;
	static char	c[2] = { 0 };
	t_reader	reader;
	t_command	cmd;

	sh_init(&g_shell, "Minishell Improved");
	while (42)
	{
		g_shell.cursor = (struct s_cursor) { 0, 0 };
		reader_init(&reader);
		command_init(&cmd);
		ft_putf(CSI "G" CSI "J" PROMPT_FG_COLOUR "%s" COLOUR_RESET " > ", g_shell.name);
		while (42)
		{
			if (cmd.string.str != NULL)
			{
				if (g_shell.cursor.y > 0)
					ft_putf(CSI "%dF", g_shell.cursor.y);
				ft_putf(CSI "%dG" CSI "J" "%s" CSI "%dG", g_shell.name_len + 4, cmd.string.str, g_shell.name_len + 4 + g_shell.cursor.x);
			}
			if (sh_getchar(&reader, STDIN_FILENO, c + 1) > 0)
			{
				sh_cli_handlers(&g_shell, &reader, &cmd, c[1], *c);
				if ((*c = c[1]) == 0x3 || *c == 0xd)
					break ;
			}
		}
		if (*c == 0xd)
			sh_exec(&cmd, &g_shell);
		ft_putchar('\n');
	}
}
