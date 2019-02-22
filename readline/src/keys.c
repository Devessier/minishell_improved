/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 13:33:26 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/22 13:39:29 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

t_ft_rl_functions	g_ft_rl_functions = { 0, { { 0, 0 } } };

bool				ft_rl_bind_key(char key, t_ft_rl_func func)
{
	const size_t	count = g_ft_rl_functions.func_count;

	if (!(g_ft_rl_functions.func_count + 1 < FT_RL_MAX_FUNC))
		return (false);
	g_ft_rl_functions.functions[count] = (t_fl_rl_func_entry) {
		.key = key,
		.fn = func,
	};
	g_ft_rl_functions.func_count++;
	return (true);
}

ssize_t				ft_rl_call_bound_functions(char c, t_readline *rl,
		t_string *line)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (i++ < g_ft_rl_functions.func_count)
		if (g_ft_rl_functions.functions[i - 1].key == c)
		{
			if (!g_ft_rl_functions.functions[i - 1].fn(c, rl, line))
			{
				ft_free_string(line);
				return (-1);
			}
			else
				count++;
		}
	return (count);
}
