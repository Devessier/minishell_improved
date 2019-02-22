/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 10:39:07 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/22 16:11:23 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READLINE_H
# define READLINE_H
# include <sys/types.h>
# include <stdbool.h>
# include <sys/ioctl.h>
# include "libft.h"
# define BUFF_SIZE 4096
# define CSI "\x1b["
# define COLOUR_RESET CSI "0m"
# define CLEAR_SCREEN CSI "2J" CSI "H"
# define FT_RL_MAX_FUNC 1 << 3

typedef enum				e_cursor_move_type
{
	GOTO_BEGINNING,
	GOTO_END,
	JUMP_TO_N_WORD,
	JUMP_TO_N_CHAR,
}							t_cursor_move_type;

typedef enum				e_ft_rl_prompt_colour
{
	RL_BLUE,
	RL_ORANGE,
	RL_RED,
}							t_ft_rl_prompt_colour;

typedef struct				s_ft_rl_reader
{
	char		buffer[BUFF_SIZE];
	ssize_t		len;
	size_t		index;
}							t_ft_rl_reader;

typedef struct				s_readline
{
	char		*prompt;
	size_t		prompt_len;
	char		*colour;
	size_t		cursor;
	bool		print_prompt;
}							t_readline;

/*
** t_ft_rl_func functions take two parameters :
** - (char)c : the character which has been written by the user
** - (t_readline *)rl : the t_readline struct which contains the cursor
** - (t_string *)line : the line written by the user, wcich can be overwritten
**		to enable autocomplete/history
*/

typedef bool				(*t_ft_rl_func)(char, t_readline *, t_string *);

typedef struct				s_ft_rl_func_entry
{
	char			key;
	t_ft_rl_func	fn;
}							t_fl_rl_func_entry;

typedef struct				s_ft_rl_functions
{
	size_t				func_count;
	t_fl_rl_func_entry	functions[FT_RL_MAX_FUNC];
}							t_ft_rl_functions;

t_string					ft_readline(char *prompt,
		t_ft_rl_prompt_colour colour);

void						ft_rl_move_cursor(t_readline *rl, t_string *string,
		t_cursor_move_type type, ssize_t move_of);
bool						ft_rl_bind_key(char key, t_ft_rl_func func);
void						ft_rl_init(void);
void						ft_rl_config_termios(int operation);

extern t_ft_rl_functions	g_ft_rl_functions;
extern bool					g_must_print_prompt;

#endif
