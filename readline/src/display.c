/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 13:24:22 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/22 13:31:39 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include "readline.h"
#include "internal.h"
#include "libft.h"

static void		reset_interval(ssize_t interval[3])
{
	*interval = 0;
	interval[1] = 0;
	interval[2] = -1;
}

static ssize_t	init_display(bool reset, ssize_t interval[3],
		t_string *line, t_readline *rl)
{
	unsigned short	prompt_end;
	unsigned short	usable_width;
	ssize_t			diff;

	prompt_end = rl->prompt_len % ft_rl_terminal_size(GET)->ws_col;
	usable_width = ft_rl_terminal_size(GET)->ws_col - prompt_end;
	if (reset)
		reset_interval(interval);
	if (interval[2] == -1)
		interval[1] = usable_width;
	diff = (ssize_t)rl->cursor - interval[2];
	if (rl->cursor < (size_t)*interval)
	{
		*interval = ft_positive(*interval + diff, 0);
		interval[1] = ft_max(interval[1] + diff, usable_width);
	}
	else if (rl->cursor >= (size_t)interval[1])
	{
		*interval = ft_min(line->len - usable_width, *interval + diff);
		interval[1] = ft_min(line->len, interval[1] + diff);
	}
	return (diff);
}

void			ft_rl_display(t_readline *rl, t_string *line, bool reset)
{
	unsigned short	prompt_end;
	static ssize_t	interval[3] = { 0, 0, -1 };
	const size_t	old_interval[2] = { interval[0], interval[1] };
	const ssize_t	diff = init_display(reset, interval, line, rl);

	prompt_end = rl->prompt_len % ft_rl_terminal_size(GET)->ws_col;
	ft_putf(CSI "%dG" CSI "J", prompt_end + 1);
	if (interval[0] > 0)
		ft_putstr("…");
	ft_putnstring(line, *interval + (interval[0] > 0), interval[1] - *interval
		- (interval[0] > 0) - ((size_t)interval[1] < line->len));
	if ((size_t)interval[1] < line->len)
		ft_putstr("…");
	if (diff < 0 && rl->cursor < old_interval[0])
		ft_putf(CSI "%dG", prompt_end + 1);
	else if (rl->cursor >= old_interval[0] && rl->cursor < old_interval[1])
		ft_putf(CSI "%dG", prompt_end + 1 + (rl->cursor - interval[0]));
	interval[2] = rl->cursor;
}
