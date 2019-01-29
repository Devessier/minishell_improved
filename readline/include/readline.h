/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 10:39:07 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/29 10:30:28 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READLINE_H
# include <sys/types.h>
# include <stdbool.h>
# include <sys/ioctl.h>
# include "libft.h"
# define READLINE_H
# define BUFF_SIZE 4096
# define CSI "\x1b["
# define COLOUR_RESET CSI "0m"
# define CLEAR_SCREEN CSI "2J" CSI "H"
# define FT_RL_MAX_FUNC 1 << 6

typedef enum				s_ft_rl_prompt_colour
{
	RL_BLUE,
	RL_ORANGE,
	RL_RED,
}							t_ft_rl_prompt_colour;

typedef struct              s_ft_rl_reader
{
    char		buffer[BUFF_SIZE];
	ssize_t		len;
	size_t		index;
}                           t_ft_rl_reader;

typedef struct              s_readline
{
    char        *prompt;
    size_t      prompt_len;
    char        *colour;
    size_t      cursor;
}               t_readline;

typedef bool                (*t_ft_rl_func)(char, char);

typedef struct              s_ft_rl_func_entry
{
    char            *name;
    t_ft_rl_func    fn;
}                           t_fl_rl_func_entry;

typedef struct              s_ft_rl_functions
{
    size_t              func_count;
    t_fl_rl_func_entry  functions[FT_RL_MAX_FUNC];
}                           t_ft_rl_functions;

t_string                    ft_readline(char *prompt, t_ft_rl_prompt_colour colour);
void                        ft_rl_init(void);
void                        ft_rl_config_termios(bool init);

extern t_ft_rl_functions    g_ft_rl_functions;

#endif
