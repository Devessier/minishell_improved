/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 13:24:22 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/24 13:24:23 by bdevessi         ###   ########.fr       */
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

void    calculate_interval(t_readline *rl, unsigned short interval[3], unsigned short last, const unsigned short move_of)
{
    const unsigned short    backward_count = (*interval - rl->cursor) / move_of + 1;
    const unsigned short    forward_count = (rl->cursor - 1[interval]) / move_of + 1;

    if (rl->cursor < last && *interval > 0 && rl->cursor <= *interval)
    {
        *interval -= move_of * backward_count;
        interval[1] -= move_of * backward_count;
    }
    else if (rl->cursor > last && rl->cursor >= interval[1])
    {
        *interval += move_of * forward_count - (forward_count != 0);
        interval[1] += move_of * forward_count - (forward_count != 0);
    }
}

void    ft_rl_display(t_readline *rl, t_string *line)
{
    const unsigned short    terminal_width = ft_rl_terminal_size(GET)->ws_col;
    const unsigned short    prompt_end = rl->prompt_len % terminal_width;
    const unsigned short    usable_width = terminal_width - prompt_end;
    //const unsigned short    fifth = usable_width / 5;
    const unsigned short    cursor_by_width = rl->cursor / usable_width;
    const unsigned short    interval[2] = { cursor_by_width * usable_width, (cursor_by_width + 1) * usable_width };

    ft_putf(CSI "%dG" CSI "J", prompt_end + 1);
    if (usable_width < 2)
        return ;
    if (usable_width == 2 && line->len > 2)
        return ft_putstr("\xF0\x9F\x98\xA5");
    if (*interval > 0)
        ft_putstr("…");
    ft_putf("len of interval = %d\n", interval[1] - *interval);
    ft_putnstring(line, *interval - (*interval > 0), interval[1] - *interval - (*interval > 0));
    if (1[interval] < line->len)
        ft_putstr("…");
    if (rl->cursor == *interval)
        ft_putf(CSI "%dG", prompt_end + 1);
    else
        ft_putf(CSI "%dG", prompt_end + 1 + (*interval > 0 ? rl->cursor - *interval + 1 : rl->cursor));
}
