/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 11:51:38 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/28 14:09:59 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"
#include "sh.h"

static int				autocomplete_state_loop(t_readline *rl,
	t_string *line, t_completion *completion)
{
	size_t	i;
	char	previous;

	i = 0;
	previous = 0;
	while (i < rl->cursor)
	{
		if (line->buff[i] == ' ' && previous == ';')
		{
			i++;
			continue ;
		}
		if (line->buff[i] == ';')
			completion->state = AC_COMMAND;
		else if ((completion->state == AC_COMMAND && line->buff[i] == ' ')
				|| (!(line->buff[i] == '\'' || line->buff[i] == '\"')
				&& completion->state != AC_COMMAND))
			completion->state = AC_FILE;
		previous = line->buff[i++];
	}
	return (i);
}

static t_completion		autocomplete_state(t_string *line, t_readline *rl)
{
	t_completion			completion;
	size_t					i;

	completion = (t_completion) { AC_COMMAND, 0, NULL };
	if (line->len == 0)
		return (completion);
	i = autocomplete_state_loop(rl, line, &completion);
	while (i > 0 && !(line->buff[i - 1] == '\''
			|| line->buff[i - 1] == '\"' || line->buff[i - 1] == ' '))
		if (line->buff[--i] != '\0')
			completion.len++;
	completion.start = &line->buff[i];
	return (completion);
}

static bool				autocomplete_init(char c,
	const t_completion *completion, t_string *line)
{
	if (line->len == 0)
		ft_putchar(BELL);
	if (c != '\t' || line->len == 0 || completion->len == 0)
		return (false);
	return (true);
}

bool					sh_autocomplete(char c, t_readline *rl, t_string *line)
{
	char				path[PATH_MAX];
	const t_completion	completion = autocomplete_state(line, rl);
	char				*filename;
	size_t				filename_len;
	size_t				diff;

	if (!autocomplete_init(c, &completion, line))
		return (true);
	diff = completion.start - line->buff;
	filename = completion.state == AC_COMMAND
		? sh_complete_command(completion.start, completion.len, path, g_env)
		: sh_complete_filename(completion.start, completion.len, path, false);
	if (filename != NULL && ft_extend_string(line,
		(filename_len = ft_strlen(filename))) && filename_len > completion.len)
	{
		ft_memmove(line->buff + diff + filename_len, line->buff + diff
			+ completion.len, line->len - (completion.len + diff));
		ft_memmove(line->buff + diff, filename, filename_len);
		line->len += filename_len - completion.len;
		ft_rl_move_cursor(rl, line, JUMP_TO_N_CHAR, filename_len
			- completion.len);
	}
	else
		ft_putchar(BELL);
	return (true);
}
