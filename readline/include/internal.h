/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 13:26:29 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/11 13:49:08 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_H
# define INTERNAL_H
# include <stdint.h>
# include <stdbool.h>
# include "readline.h"
# define BELL 0x7

typedef enum                    e_terminal_size_operation
{
	GET,
	SET,
}                               t_terminal_size_operation;

typedef enum                    e_delete_type
{
	DELETE_CURR_CHAR,
	DELETE_PREV_CHAR,
}                               t_delete_type;

uint8_t                         get_utf8_sequence_len(uint8_t c);

int                             ft_rl_getchar(t_ft_rl_reader *this, const int fd, char *c);
int                             ft_rl_getchar_blocking(t_ft_rl_reader *this, const int fd, char *c);
void                            ft_rl_display(t_readline *rl, t_string *line, bool reset);
bool                            ft_rl_concat_strings_cursor(t_string *string, t_readline *rl, char c);
void                            ft_rl_delete_char(t_readline *rl, t_string *string, t_delete_type type);
void                            ft_rl_move_cursor(t_readline *rl, t_string *string, t_cursor_move_type type, ssize_t move_of);
bool                            ft_rl_handle_character(t_readline *rl, t_ft_rl_reader *reader, t_string *string, char characters[2]);
bool                            ft_rl_run_func(char *key, char curr, char prev);
bool                            ft_rl_exit(char curr, char prev);
struct winsize                  *ft_rl_terminal_size(t_terminal_size_operation operation);
bool                            ft_rl_internal_checks(void);

#endif
