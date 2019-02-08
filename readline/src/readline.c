/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 10:46:02 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/08 16:32:16 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "readline.h"
#include "internal.h"
#include "libft.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

t_ft_rl_functions   g_ft_rl_functions = { 0, { { 0, 0 } } };

static void init_ft_rl_reader_string(t_ft_rl_reader *reader, t_string *string)
{
	*reader = (t_ft_rl_reader) {
		.len = 0,
		.index = 0,
	};
	ft_bzero(reader->buffer, sizeof(reader->buffer));
	*string = (t_string) {
		.len = 0,
		.cap = 0,
		.buff = NULL,
	};
}

char		*ft_rl_prompt_colour(t_ft_rl_prompt_colour colour)
{
	if (colour == RL_BLUE)
		return (CSI "38;5;32m");
	if (colour == RL_ORANGE)
		return (CSI "38;5;208m");
	return (CSI "38;5;196m");
}

bool		ft_rl_bind_key(char key, t_ft_rl_func func)
{
	if (!(g_ft_rl_functions.func_count + 1 < FT_RL_MAX_FUNC))
		return (false);
	g_ft_rl_functions.functions[g_ft_rl_functions.func_count++] = (t_fl_rl_func_entry) {
		.key = key,
		.fn = func,
	};
	return (true);
}

ssize_t		ft_rl_call_bound_functions(char c, t_readline *rl, t_string *line)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (i++ < g_ft_rl_functions.func_count)
	{
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
	}
	return (count);
}

t_string    ft_readline(char *prompt, t_ft_rl_prompt_colour colour)
{
	const t_readline    rl = { prompt, ft_strlen(prompt) + 3, ft_rl_prompt_colour(colour), 0 };
	t_ft_rl_reader      reader;
	char                characters[2];
	t_string            string;
	bool				reset;

	reset = true;
	init_ft_rl_reader_string(&reader, &string);
	ft_bzero(characters, sizeof(characters));
	ft_putf_fd(STDERR_FILENO, "%s%s $ " COLOUR_RESET, rl.colour, rl.prompt);
	while (42)
	{
		if (ft_rl_internal_checks())
			reset = true;
		if (string.buff != NULL)
			ft_rl_display((t_readline *)&rl, &string, reset);
		if (ft_rl_getchar(&reader, 0, characters + 1) > 0)
		{
			if (ft_rl_call_bound_functions(characters[1], (t_readline *)&rl, &string) == -1)
				return (string);
			ft_rl_handle_character((t_readline *)&rl, &reader, &string, characters);
			if ((*characters = characters[1]) == 0x3 || *characters == 0xd || *characters == 0xA)
			{
				if (*characters == 0x3)
					ft_free_string(&string);
				break ;
			}
		}
		reset = false;
	}
	ft_putchar('\n');
	return (string);
}

bool        ft_rl_handle_character(t_readline *rl, t_ft_rl_reader *reader, t_string *string, char characters[2])
{
	char    next_c;

	if (characters[1] == 0x1b)
	{
		ft_rl_getchar_blocking(reader, 0, &next_c);
		if (next_c == '[')
		{
			ft_rl_getchar_blocking(reader, 0, &next_c);
			if (ft_isdigit(next_c))
				ft_rl_getchar_blocking(reader, 0, &next_c);
			else if (next_c == 'C' || next_c == 'D')
				ft_rl_move_cursor(rl, string, JUMP_TO_N_CHAR, next_c == 'C' ? 1 : -1);
			else if (next_c == 'A' || next_c == 'B')
				;// vertical move, move in the history
		}
	}
	else if (characters[1] == 0x7f || (characters[1] == 0x4 && rl->cursor < string->len))
		ft_rl_delete_char(rl, string, characters[1] == 0x4 ? DELETE_CURR_CHAR : DELETE_PREV_CHAR);
	else if (characters[1] == 0x5 || characters[1] == 0x1)
		ft_rl_move_cursor(rl, string, characters[1] == 0x1 ? GOTO_BEGINNING : GOTO_END, 0);
	else if (ft_isprint(characters[1]) && (characters[1] != ' ' || (rl->cursor > 0 && string->buff[rl->cursor - 1] != ' ')))
	{
		if (!ft_rl_concat_strings_cursor(string, rl, characters[1]))
			return (false);
		ft_rl_move_cursor(rl, string, JUMP_TO_N_CHAR, 1);
	}
	else if (characters[1] == 0xC)
		ft_putf_fd(STDERR_FILENO, CLEAR_SCREEN "%s%s $ " COLOUR_RESET, rl->colour, rl->prompt);
	else if (characters[1] == 0x4)
		ft_putchar(BELL);
	return (true);
}

bool        ft_rl_exit(char curr, char prev)
{
	(void)curr, (void)prev;
	// free what has to be freed
	ft_putf("exit\n");
	exit(0);
}
