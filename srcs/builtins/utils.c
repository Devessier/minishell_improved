/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 09:45:43 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/04 09:46:28 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include <stdbool.h>

bool	sh_builtin_exit(t_string *args, t_env *env)
{
	(void)args, (void)env;
	return (true);
}

bool	sh_builtin_echo(t_string *args, t_env *env)
{
	(void)args, (void)env;
	return (true);
}

bool	sh_builtin_cd(t_string *args, t_env *env)
{
	(void)args, (void)env;
	return (true);
}

bool	sh_builtin_which(t_string *args, t_env *env)
{
	(void)args, (void)env;
	return (true);
}
