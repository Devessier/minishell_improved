/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 12:40:28 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/22 13:18:23 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "internal.h"

bool	ft_rl_concat_strings_cursor(t_string *string, t_readline *rl, char c)
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

void	ft_rl_delete_char(t_readline *rl, t_string *string, t_delete_type type)
{
	if ((type == DELETE_PREV_CHAR && rl->cursor == 0)
			|| (type == DELETE_CURR_CHAR && string->len > 0
				&& rl->cursor == string->len))
		return (ft_putchar(BELL));
	ft_strcpy(string->buff + rl->cursor - (type == DELETE_PREV_CHAR),
			string->buff + rl->cursor + (type == DELETE_CURR_CHAR));
	string->len--;
	if (type == DELETE_PREV_CHAR)
		ft_rl_move_cursor(rl, string, JUMP_TO_N_CHAR, -1);
}

void	ft_rl_move_cursor(t_readline *rl, t_string *string,
		t_cursor_move_type type, ssize_t move_of)
{
	const int8_t	sign = move_of > 0 ? 1 : -1;

	if (type == GOTO_BEGINNING)
		rl->cursor = 0;
	else if (type == GOTO_END)
		rl->cursor = string->len;
	else if (type == JUMP_TO_N_CHAR && move_of != 0)
	{
		if ((ssize_t)rl->cursor + move_of < 0
				|| rl->cursor + move_of > string->len)
		{
			return (ft_rl_move_cursor(rl, string, sign == 1
						? GOTO_END : GOTO_BEGINNING, 0));
		}
		rl->cursor += move_of;
	}
}
