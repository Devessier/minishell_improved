/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 12:07:57 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/21 13:24:40 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include "internal.h"

static bool					g_ot_sigwinch = false;
static struct termios		g_original_termios_struct;

static void					handle_sigwinch(int sig)
{
	if (sig != SIGWINCH)
		return ;
	g_ot_sigwinch = true;
}

struct winsize				*ft_rl_terminal_size(t_termsiz_op operation)
{
	static struct winsize	ws;

	if (operation == GET)
		return (&ws);
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	if (ws.ws_col == 0)
		ws.ws_col = 80;
	return (&ws);
}

void						ft_rl_init(void)
{
	signal(SIGWINCH, handle_sigwinch);
	ft_rl_terminal_size(SET);
	ft_rl_config_termios(1);
}

bool						ft_rl_internal_checks(void)
{
	if (g_ot_sigwinch)
	{
		ft_rl_terminal_size(SET);
		g_ot_sigwinch = false;
		return (true);
	}
	return (false);
}

void						ft_rl_config_termios(int operation)
{
	struct termios	raw_termios_struct;

	if (operation == 0)
		tcsetattr(0, TCSAFLUSH, &g_original_termios_struct);
	else if (operation == 1)
	{
		raw_termios_struct = g_original_termios_struct;
		raw_termios_struct.c_iflag &= ~(ICRNL | IXON);
		raw_termios_struct.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
		raw_termios_struct.c_cc[VMIN] = 0;
		raw_termios_struct.c_cc[VTIME] = 1;
		tcsetattr(0, TCSAFLUSH, &raw_termios_struct);
	}
	else
		tcgetattr(0, &g_original_termios_struct);
}
