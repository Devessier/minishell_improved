/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 17:09:14 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/04 16:50:09 by bdevessi         ###   ########.fr       */
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
	(void)argc, (void)argv;
	const t_env	env = copy_env(envp);
	int			status;

	status = 0;
	ft_rl_config_termios(2);
	while (42)
	{
		ft_rl_init();
		t_string string = ft_readline("Minishell Improved", status == 0 ? RL_BLUE : RL_RED);
		if (string.len > 0)
		{
			ft_rl_config_termios(0);
			status = sh_exec(&string, (t_env *)&env);
		}
	}
	ft_rl_config_termios(0);
}
