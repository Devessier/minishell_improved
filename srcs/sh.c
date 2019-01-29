/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 17:09:14 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/29 17:37:21 by bdevessi         ###   ########.fr       */
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

int		main(int argc, char **argv, char **envp)
{
	(void)argc, (void)argv, (void)envp;

	while (42)
	{
		ft_rl_init();
		t_string string = ft_readline("Minishell Improved", RL_ORANGE);
		if (string.buff != NULL)
			sh_exec(&string);
		ft_rl_config_termios(false);
	}
	ft_rl_config_termios(false);
}
