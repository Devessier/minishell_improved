/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/03 21:03:18 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/04 09:46:49 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include <stdbool.h>

bool	sh_builtin_env(t_string *args, t_env *env)
{
	(void)args, (void)env;
	return (true);
}

bool	sh_builtin_setenv(t_string *args, t_env *env)
{
	(void)args, (void)env;
	return (true);
}

bool	sh_builtin_unsetenv(t_string *args, t_env *env)
{
	(void)args, (void)env;
	return (true);
}
