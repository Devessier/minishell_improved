/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 10:46:02 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/22 16:25:34 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include "readline.h"
#include "internal.h"
#include "libft.h"

bool		g_must_print_prompt = false;

static int	ft_readline_loop(t_readline *rl, t_string *line,
		char characters[3], t_ft_rl_reader *reader)
{
	static bool	reset = true;
	int			getchar_result;

	if (ft_rl_internal_checks())
		reset = true;
	if (line->buff != NULL && g_must_print_prompt)
		ft_rl_display(rl, line, reset);
	if ((getchar_result = ft_rl_getchar(reader, 0, characters + 1)) > 0)
	{
		if (ft_rl_call_bound_functions(characters[1], rl, line) == -1)
			return (2);
		if (!ft_rl_handle_character(rl, reader, line, characters))
			return (1);
		if ((*characters = characters[1]) == 0x3 || *characters == 0xd\
			|| *characters == 0xA)
		{
			if (*characters == 0x3)
				ft_free_string(line);
			return (1);
		}
	}
	else if (getchar_result == -1)
		return (1);
	reset = false;
	return (0);
}

t_string	ft_readline(char *prompt, t_ft_rl_prompt_colour colour)
{
	t_readline		rl;
	t_ft_rl_reader	reader;
	char			characters[2];
	t_string		line;
	int				result;

	g_must_print_prompt = isatty(0) && isatty(1);
	rl = (t_readline) {
		prompt,
		ft_strlen(prompt) + 3,
		ft_rl_prompt_colour(colour),
		0,
		false,
	};
	if (g_must_print_prompt)
		write(1, CSI "6n", 4);
	init_ft_rl_reader_string(&reader, &line);
	ft_bzero(characters, sizeof(characters));
	while (42)
		if (rl.print_prompt && g_must_print_prompt)
		{
			ft_putf_fd(2, "%s%s $ " COLOUR_RESET, rl.colour, rl.prompt);
			rl.print_prompt = false;
		}
		else if ((result = ft_readline_loop((t_readline *)&rl,
			&line, characters, &reader)) == 2)
			return (line);
		else if (result == 1)
			break ;
	if (g_must_print_prompt)
		ft_putchar('\n');
	return (line);
}

static bool	ft_rl_handle_escaped_chars(t_readline *rl,
		t_ft_rl_reader *reader, t_string *string)
{
	char	next_c;
	size_t	i;
	char	column[2];
	int		status;

	next_c = '\0';
	i = 0;
	if (ft_rl_getchar_blocking(reader, 0, &next_c) < 0 || next_c != '[')
		return (next_c != '[');
	if (ft_rl_getchar_blocking(reader, 0, &next_c) == -1)
		return (false);
	while (i < 3 && ft_isdigit(next_c))
	{
		if (ft_rl_getchar_blocking(reader, 0, &next_c) < 0)
			return (false);
		i++;
	}
	if (next_c == 'C' || next_c == 'D')
	{
		ft_rl_move_cursor(rl, string, JUMP_TO_N_CHAR, next_c == 'C' ? 1 : -1);
		return (true);
	}
	if (next_c == ';')
	{
		while ((status = ft_rl_getchar_blocking(reader, 0, &next_c)) > 0 && ft_isdigit(next_c))
			*column = next_c;
		1[column] = '\0';
		if (status < 0)
			return (false);
		else if (next_c == 'R')
		{
			if (ft_atoi(column) != 1)
				ft_putstr_fd(CSI "330;107;m" "%\n" CSI "0m", 2);
			rl->print_prompt = true;
		}
	}
	return (true);
}

bool		ft_rl_handle_character(t_readline *rl, t_ft_rl_reader *reader,
		t_string *string, char characters[2])
{
	if (characters[1] == 0x1b)
		return (ft_rl_handle_escaped_chars(rl, reader, string));
	else if (characters[1] == 0x7f || (characters[1] == 0x4
			&& rl->cursor < string->len))
		ft_rl_delete_char(rl, string, characters[1] == 0x4
			? DELETE_CURR_CHAR : DELETE_PREV_CHAR);
	else if (characters[1] == 0x5 || characters[1] == 0x1)
		ft_rl_move_cursor(rl, string, characters[1] == 0x1
			? GOTO_BEGINNING : GOTO_END, 0);
	else if (ft_isprint(characters[1]) && (characters[1] != ' ' ||
		(rl->cursor > 0 && string->buff[rl->cursor - 1] != ' ')))
	{
		if (!ft_rl_concat_strings_cursor(string, rl, characters[1]))
			return (false);
		ft_rl_move_cursor(rl, string, JUMP_TO_N_CHAR, 1);
	}
	else if (characters[1] == 0xC)
		ft_putf_fd(STDERR_FILENO, CLEAR_SCREEN "%s%s $ " COLOUR_RESET,
			rl->colour, rl->prompt);
	else if (characters[1] == 0x4)
		ft_putchar(BELL);
	return (true);
}
