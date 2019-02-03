/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 17:09:14 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/03 19:04:26 by bdevessi         ###   ########.fr       */
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

	print_env(&env);
	put_env((t_env *)&env, "TEUB", "qfjk ghqkfjghkqjfh jkqh gkjqhfgkqjfhg kjqhfg jkqdhg jkqfhg jkqhdfg jkqfhg jkqdfh gjkqhfg ");
	print_env(&env);
	ft_putstr("\n\n");
	put_env((t_env *)&env, "TEST", "minishell");
	unset_env((t_env *)&env, "TEUB");
	put_env((t_env *)&env, "BAPTISTE", "DEVESSIER");
	print_env(&env);
	ft_putstr("\n\n");
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
