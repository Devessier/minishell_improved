/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:29:24 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/01 17:40:01 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_H
# define SH_H
# include <sys/types.h>
# include <stdint.h>
# include <stdbool.h>
# include <sys/ioctl.h>
# include "libft.h"
# define ENV_SEPARATOR '\1'

typedef enum		e_token_char
{
	T_WORD,
	T_ESCAPE,
	T_DQUOTE,
	T_SQUOTE,
	T_WHITESPACE,
	T_SEMICOLON,
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
	IN_DQUOTE = 2,
	IN_SQUOTE = 3,
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

typedef struct		s_ast_node
{
	enum			{
		ROOT,
		COMMAND,
		ARG
	}				tag;
	union			{
		struct		{
			size_t				len;
			size_t				cap;
			struct s_ast_node	*commands;
		}			root;
		struct		{
			t_string			string;
			t_string			args;
			bool				dirty;
		}			command;
	}				payload;
}					t_ast_node;

typedef enum		e_color
{
	BLUE,
	RED,
	ORANGE
}					t_color;

t_lexer				sh_lexer(t_string *string);
bool				destroy_lexer(const t_lexer *lexer);

int					sh_exec(t_string *string);
t_ast_node			sh_construct_ast(const t_lexer *lexer);

void				print_ast(t_ast_node root);

char				*sh_get_env(char *env_var, size_t len);

#endif
