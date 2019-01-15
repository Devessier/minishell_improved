/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:29:24 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/15 16:11:46 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_H
# define SH_H
# include <sys/types.h>
# include <stdint.h>
# include <stdbool.h>
# define SHELL_NAME_LEN 1 << 8
# define BUFF_SIZE 4096
# define CSI "\x1b["
# define RESET_COL_POS CSI "0G"
# define ERASE_LINE CSI "1K"
# define LEFT_CURSOR CSI "C"
# define RIGHT_CURSOR CSI "D"
# define PROMPT_FG_COLOUR CSI "38;5;45m"
# define COLOUR_RESET CSI "0m"

typedef struct		s_tring
{
	size_t	len;
	size_t	cap;
	char	*str;
}					t_string;

typedef enum		e_token_char
{
	T_LAMBDA,
	T_DQUOTE,
	T_SQUOTE,
	T_WHITESPACE,
	T_SEMICOLON,
	T_ESCAPE,
}					t_oken_char;

typedef struct		s_token
{
	string		payload;
	t_oken_char	type;
}					t_oken;

typedef enum		e_lexer_state
{
	IN_DQUOTE,
	IN_SQUOTE,
	GLOBAL_SCOPE
}					t_lexer_state;

typedef struct		s_lexer
{
	size_t			len;
	t_lexer_state	state;
	t_oken			tokens[];
}					t_lexer;

struct				s_cursor
{
	uint8_t			x;
	uint8_t			y;
};

typedef enum		e_color
{
	BLUE,
	RED,
	ORANGE
}					t_color;

typedef struct		s_sh
{
	struct s_cursor	cursor;
	char			name[SHELL_NAME_LEN];
	size_t			name_len;
	t_color			primary_color;
}					t_sh;

typedef struct		s_reader
{
	char			buffer[BUFF_SIZE];
	ssize_t			len;
	size_t			index;
}					t_reader;

typedef struct		s_command
{
	size_t			cursor;
	t_string		string;
}					t_command;

void				reader_init(t_reader *this);
void				command_init(t_command *this);
void				move_cursor(t_command *this, t_sh *shell, ssize_t move);
int					sh_command_cat(t_command *this, t_sh *sh, char c);
void				sh_command_del(t_command *cmd, t_sh *sh, bool current);
ssize_t				sh_getchar(t_reader *this, const int fd, char *c);
bool				sh_lexer(t_command *cmd);
int					sh_exec(t_command *cmd, t_sh *sh);

t_string			new_string(char *c, bool char_mode);
bool				concat_strings(t_string *this, char *str);
bool				extend_string(t_string *this, size_t size);

#endif
