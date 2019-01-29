/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:29:24 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/29 17:36:44 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_H
# define SH_H
# include <sys/types.h>
# include <stdint.h>
# include <stdbool.h>
# include <sys/ioctl.h>
# include "libft.h"

typedef enum		e_token_char
{
	T_WORD,
	T_DQUOTE,
	T_SQUOTE,
	T_WHITESPACE,
	T_SEMICOLON,
	T_ESCAPE,
	T_AMPERSAND,
	T_PIPE,
}					t_oken_char;

typedef struct		s_token
{
	t_string	payload;
	t_oken_char	type;
}					t_oken;

typedef enum		e_lexer_state
{
	IN_DQUOTE = 1,
	IN_SQUOTE = 2,
	GLOBAL_SCOPE,
	EXPLICIT_SYNTAX_ERROR,
}					t_lexer_state;

typedef struct		s_lexer
{
	size_t			len;
	size_t			cap;
	t_lexer_state	state;
	t_oken			*tokens;
}					t_lexer;

typedef enum		e_color
{
	BLUE,
	RED,
	ORANGE
}					t_color;

t_lexer				sh_lexer(t_string *string);
int					sh_exec(t_string *string);

char				*sh_get_env(char *env_var, size_t len);

#endif
