/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 12:07:57 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/24 12:07:58 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include "internal.h"

static bool                 got_sigwinch = false;

static void                 handle_sigwinch(int sig)
{
    if (sig != SIGWINCH)
        return ;
    got_sigwinch = true;
}

struct winsize              *ft_rl_terminal_size(t_terminal_size_operation operation)
{
    static struct winsize   ws;

    if (operation == GET)
        return (&ws);
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    return (&ws);
}

void                        ft_rl_init(void)
{
    signal(SIGWINCH, handle_sigwinch);
    ft_rl_terminal_size(SET);
    ft_rl_config_termios(true);
}

void                        ft_rl_internal_checks(void)
{
    if (got_sigwinch)
    {
        ft_rl_terminal_size(SET);
        got_sigwinch = false;
    }
}

void                        ft_rl_config_termios(bool init)
{
    static struct termios   original_termios_struct;
    struct termios          raw_termios_struct;

    if (init)
    {
        tcgetattr(0, &original_termios_struct);
        raw_termios_struct = original_termios_struct;
        raw_termios_struct.c_iflag &= ~(ICRNL | IXON);
        raw_termios_struct.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
        raw_termios_struct.c_cc[VMIN] = 0;
        raw_termios_struct.c_cc[VTIME] = 1;
        tcsetattr(0, TCSAFLUSH, &raw_termios_struct);
    }
    else
        tcsetattr(0, TCSAFLUSH, &original_termios_struct);
}
