/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:29:24 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/11 19:11:50 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_H
# define SH_H
# include <sys/types.h>
# include <stdint.h>
# define SHELL_NAME_LEN 1 << 8
# define BUFF_SIZE 4096
# define CSI "\x1b["
# define RESET_COL_POS CSI "0G"
# define ERASE_LINE CSI "1K"
# define LEFT_CURSOR CSI "C"
# define RIGHT_CURSOR CSI "D"
# define PROMPT_FG_COLOUR CSI "38;5;45m"
# define COLOUR_RESET CSI "0m"

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
	size_t			len;
	size_t			cap;
	size_t			cursor;
	char			*str;
}					t_command;

void				reader_init(t_reader *this);
void				command_init(t_command *this);
int					sh_command_cat(t_command *this, t_sh *sh, char c);
void				sh_command_del(t_command *cmd);
ssize_t				sh_getchar(t_reader *this, const int fd, char *c);

#endif
