/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:29:24 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/11 13:19:40 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_H
# define SH_H
# include <sys/types.h>
# include <stdint.h>
# include <stdbool.h>
# include "libft.h"

typedef enum			e_token_char
{
	T_WORD,
	T_ESCAPE,
	T_DQUOTE,
	T_SQUOTE,
	T_WHITESPACE,
	T_SEMICOLON,
	T_AMPERSAND,
	T_PIPE,
}						t_oken_char;

typedef struct			s_token
{
	t_string	payload;
	t_oken_char	type;
}						t_oken;

typedef enum			e_lexer_state
{
	IN_DQUOTE = 2,
	IN_SQUOTE = 3,
	GLOBAL_SCOPE,
	EXPLICIT_SYNTAX_ERROR,
}						t_lexer_state;

typedef struct			s_lexer
{
	size_t			len;
	size_t			cap;
	t_lexer_state	state;
	t_oken			*tokens;
}						t_lexer;

typedef struct			s_ast_node
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
			size_t				len;
			size_t				cap;
			t_string			*args;
			bool				dirty;
		}			command;
	}				payload;
}						t_ast_node;

typedef struct			s_env
{
	size_t		len;
	size_t		cap;
	t_string	*vars;
}						t_env;

typedef enum			e_color
{
	BLUE,
	RED,
	ORANGE
}						t_color;

typedef int				(*t_shell_builtin_fn)(t_string *, size_t, t_env *);

typedef struct			s_shell_builtin
{
	char				*name;
	t_shell_builtin_fn	fn;
}						t_shell_builtin;

typedef enum			s_lookup_result
{
	LK_NOT_FOUND,
	LK_BUILTIN,
	LK_FOUND,
	LK_NO_RIGHTS,
	LK_PATH_TOO_LONG,
}						t_lookup_result;

t_lexer					sh_lexer(t_string *string, t_env *env);
bool					destroy_lexer(const t_lexer *lexer);

int						sh_exec(t_string *string, t_env *env);
t_ast_node				sh_construct_ast(const t_lexer *lexer);

void					print_ast(t_ast_node root);

t_env					copy_env(char **envp);
bool					put_env(t_env *env, const char *name, const char *value);
void					print_env(const t_env *env);
bool					unset_env(t_env *env, const char *name);
t_string				get_env(t_env *env, const char *name);

char					*sh_complete_command(char *start, size_t len, t_env *env);
char					*sh_complete_filename(char *start, size_t len);

/*
** Shell builtins : mandatory and optional (bonus)
*/

int						sh_builtin_exit(t_string *args, size_t len, t_env *env);
int						sh_builtin_env(t_string *args, size_t len, t_env *env);
int						sh_builtin_setenv(t_string *args, size_t len, t_env *env);
int				 		sh_builtin_unsetenv(t_string *args, size_t len, t_env *env);
int						sh_builtin_echo(t_string *args, size_t len, t_env *env);
int						sh_builtin_cd(t_string *args, size_t len, t_env *env);
int						sh_builtin_which(t_string *args, size_t len, t_env *env);

extern t_shell_builtin	sh_builtins[];

#endif
