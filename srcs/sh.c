/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 17:09:14 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/14 09:26:01 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "sh.h"
#include "libft.h"

struct termios	g_original_termios;

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

void	move_cursor(t_command *cmd, t_sh *shell, ssize_t move)
{
	if ((move < 0 && shell->cursor.x + move >= 0)
			|| (move > 0 && shell->cursor.x + (size_t)move <= cmd->len))
		shell->cursor.x += move;
}

void	sh_cli_handlers(t_sh *shell, t_reader *reader, t_command *cmd, char c, char prev)
{
	char	cnext;

	if (c == 0x4 && cmd->len == 0)
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
	else if (c == 0x7f || (c == 0x4 && shell->cursor.x < cmd->len))
		sh_command_del(cmd, shell, c == 0x7f);
	else if (c == 0x5 || c == 0x1)
		move_cursor(cmd, shell, c == 0x1 ? -shell->cursor.x : cmd->len - shell->cursor.x);
	else if (ft_isprint(c) && ((c == ' ' && prev != ' ') || c != ' '))
		sh_command_cat(cmd, shell, c);
	else if (c == 0x4)
		ft_putchar(0x7);
}

void	sh_init(t_sh *sh, const char *name)
{
	*sh = (t_sh) {
		.cursor = { 0, 0 },
		.primary_color = BLUE,
	};
	ft_strcpy(sh->name, name);
	sh->name_len = ft_strlen(sh->name);
	sh_setup_raw_mode();
}

int		main(int argc, char **argv, char **envp)
{
	(void)argc, (void)argv, (void)envp;
	static char	c[2] = { 0 };
	t_sh		shell;
	t_reader	reader;
	t_command	cmd;

	sh_init(&shell, "Minishell Improved");
	while (42)
	{
		shell.cursor = (struct s_cursor) { 0, 0 };
		reader_init(&reader);
		command_init(&cmd);
		ft_putf(CSI "G" CSI "K" PROMPT_FG_COLOUR "%s" COLOUR_RESET " > ", shell.name);
		while (42)
		{
			if (cmd.str != NULL)
				ft_putf(CSI "%dG" CSI "K" "%s" CSI "%dG", shell.name_len + 4, cmd.str, shell.name_len + 4 + shell.cursor.x);
			if (sh_getchar(&reader, STDIN_FILENO, c + 1) > 0)
			{
				sh_cli_handlers(&shell, &reader, &cmd, c[1], *c);
				if ((*c = c[1]) == 0x3)
					break ;
			}
		}
		ft_putchar('\n');
	}
}
