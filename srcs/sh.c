/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 17:09:14 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/21 15:58:10 by bdevessi         ###   ########.fr       */
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

/*void	update_window_size(int sig)
{
	if (sig != SIGWINCH)
		return ;
	ft_putf("update\n");
	ioctl(STDIN_FILENO, TIOCGWINSZ, &g_shell.ws);
}*/

int		main(int argc, char **argv, char **envp)
{
	(void)argc, (void)argv, (void)envp;

    ft_rl_init();
	while (42)
	{
        t_string string = ft_readline("Minishell Improved", PROMPT_FG_COLOUR);
        if (string.buff != NULL)
            ft_putf("string = %s\n", string.buff);
	}
    ft_rl_config_termios(false);
}
