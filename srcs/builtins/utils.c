/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 09:45:43 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/14 11:21:56 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "readline.h"
#include <stdbool.h>

int		sh_builtin_echo(t_string *args, size_t len, t_env *env)
{
	(void)env;
	const bool	endl = !(len > 0 && ft_strcmp(args[0].buff, "-n") == 0);
	size_t	i;

	i = !endl;
	while (i++ < len)
		ft_putf("%s%c", args[i - 1].buff, i != len ? ' ' : '\0');
	if (endl)
		ft_putchar('\n');
	else
		ft_putstr_fd(CSI "30;107;m" "%\n" CSI "0m", 2);
	return (0);
}

int		sh_builtin_cd(t_string *args, size_t len, t_env *env)
{
	(void)args, (void)len, (void)env;
	return (true);
}

int		sh_builtin_which(t_string *args, size_t len, t_env *env)
{
	(void)args, (void)len, (void)env;
	return (true);
}
