/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 13:24:22 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/29 16:20:11 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"
#include "internal.h"
#include "libft.h"
#include <unistd.h>

bool    ft_rl_concat_strings_cursor(t_string *string, t_readline *rl, char c)
{
	if (!ft_extend_string(string, 1))
		return (false);
	if (rl->cursor == string->len)
		string->buff[string->len++] = c;
	else
	{
		ft_memmove(string->buff + rl->cursor + 1,
				string->buff + rl->cursor, string->len - rl->cursor);
		string->buff[rl->cursor] = c;
		string->len++;
	}
	return (true);
}

void    ft_rl_delete_char(t_readline *rl, t_string *string, t_delete_type type)
{
	if ((type == DELETE_PREV_CHAR && rl->cursor == 0)
			|| (type == DELETE_CURR_CHAR && string->len > 0 && rl->cursor == string->len))
		return ft_putchar(BELL);
	ft_strcpy(string->buff + rl->cursor - (type == DELETE_PREV_CHAR),
			string->buff + rl->cursor + (type == DELETE_CURR_CHAR));
	string->len--;
	if (type == DELETE_PREV_CHAR)
		ft_rl_move_cursor(rl, string, JUMP_TO_N_CHAR, -1);
}

void    ft_rl_move_cursor(t_readline *rl, t_string *string, t_cursor_move_type type, ssize_t move_of)
{
	const int8_t    sign = move_of > 0 ? 1 : -1;
	char            *pointer;

	if (type == GOTO_BEGINNING)
		rl->cursor = 0;
	else if (type == GOTO_END)
		rl->cursor = string->len;
	else if (type == JUMP_TO_N_WORD)
	{
		while (move_of != 0)
		{
			if ((pointer = (sign == 1 ? ft_strchr : ft_strrchr)(string->buff + rl->cursor, ' ')) == NULL
					|| pointer < string->buff)
				return ft_rl_move_cursor(rl, string, sign == 1 ? GOTO_END : GOTO_BEGINNING, 0);
			rl->cursor += pointer - string->buff + sign == 1;
			move_of += sign;
		}
	}
	else if (type == JUMP_TO_N_CHAR && move_of != 0)
	{
		if ((ssize_t)rl->cursor + move_of < 0 || rl->cursor + move_of > string->len)
			return ft_rl_move_cursor(rl, string, sign == 1 ? GOTO_END : GOTO_BEGINNING, 0);
		rl->cursor += move_of;
	}
}

void	reset_interval(ssize_t interval[3])
{
	*interval = 0;
	interval[1] = 0;
	interval[2] = -1;
}

ssize_t	ft_max(ssize_t a, ssize_t b)
{
	return ((a > b) ? a : b);
}

ssize_t	ft_positive(ssize_t a, ssize_t b)
{
	if (a < 0 && b < 0)
		return (0);
	if (a < 0)
		return (b);
	return (a);
}

void    ft_rl_display(t_readline *rl, t_string *line, bool reset)
{
	const unsigned short    prompt_end = rl->prompt_len % ft_rl_terminal_size(GET)->ws_col;
	const unsigned short    usable_width = ft_rl_terminal_size(GET)->ws_col - prompt_end;
	static ssize_t			interval[3] = { 0, 0, -1 };
	const size_t			old_interval[2] = { interval[0], interval[1] };
	ssize_t					diff;

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
	ft_putf(CSI "%dG" CSI "J", prompt_end + 1);
	if (interval[0] > 0)
		ft_putstr("…");
	ft_putnstring(line, *interval + (interval[0] > 0), interval[1] - *interval - (interval[0] > 0) - ((size_t)interval[1] < line->len));
	if ((size_t)interval[1] < line->len)
		ft_putstr("…");
	if (diff < 0 && rl->cursor < old_interval[0])
		ft_putf(CSI "%dG", prompt_end + 1);
	else if (rl->cursor >= old_interval[0] && rl->cursor < old_interval[1])
		ft_putf(CSI "%dG", prompt_end + 1 + (rl->cursor - interval[0]));
	interval[2] = rl->cursor;
}
