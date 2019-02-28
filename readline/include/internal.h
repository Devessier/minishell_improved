/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 13:26:29 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/25 14:53:32 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_H
# define INTERNAL_H
# include <stdbool.h>
# include "readline.h"
# define BELL 0x7
# define EURO "\xE2\x82\xAC"

typedef enum	e_terminal_size_operation
{
	GET,
	SET,
}				t_termsiz_op;

typedef enum	e_delete_type
{
	DELETE_CURR_CHAR,
	DELETE_PREV_CHAR,
}				t_delete_type;

void			init_ft_rl_reader_string(t_ft_rl_reader *reader,
		t_string *string);
ssize_t			ft_rl_call_bound_functions(char c, t_readline *rl,
		t_string *line);
char			*ft_rl_prompt_colour(t_ft_rl_prompt_colour colour);
int				ft_rl_getchar(t_ft_rl_reader *this, const int fd,
		char *c);
int				ft_rl_getchar_blocking(t_ft_rl_reader *this, const int fd,
		char *c);
void			ft_rl_display(t_readline *rl, t_string *line, bool reset);
bool			ft_rl_concat_strings_cursor(t_string *string, t_readline *rl,
		char c);
void			ft_rl_delete_char(t_readline *rl, t_string *string,
		t_delete_type type);
void			ft_rl_move_cursor(t_readline *rl, t_string *string,
		t_cursor_move_type type, ssize_t move_of);
bool			ft_rl_handle_character(t_readline *rl, t_ft_rl_reader *reader,
		t_string *string, char characters[2]);
bool			ft_rl_run_func(char *key, char curr, char prev);
bool			ft_rl_exit(char curr, char prev);
struct winsize	*ft_rl_terminal_size(t_termsiz_op operation);
bool			ft_rl_internal_checks(void);

#endif
