/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/21 16:55:55 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/21 16:56:42 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

char	*ft_rl_prompt_colour(t_ft_rl_prompt_colour colour)
{
	if (colour == RL_BLUE)
		return (CSI "38;5;32m");
	if (colour == RL_ORANGE)
		return (CSI "38;5;208m");
	return (CSI "38;5;196m");
}
