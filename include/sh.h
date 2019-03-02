/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 20:29:24 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/02 12:55:41 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_H
# define SH_H
# include <sys/types.h>
# include <stdint.h>
# include <stdbool.h>
# include <sys/syslimits.h>
# include "libft.h"
# include "readline.h"
# define PROMPT_START "Minishell Improved"
# define BELL 0x7

/*
** -- Environment variables --
*/

typedef struct			s_env
{
	size_t		len;
	size_t		cap;
	t_string	*vars;
}						t_env;

t_env					copy_env(char **envp);
bool					put_env(t_env *env, const char *name,
	const char *value);
void					print_env(const t_env *env);
bool					unset_env(t_env *env, const char *name);
t_string				get_env(t_env *env, const char *name, size_t len);
bool					destroy_env(t_env *env);

/*
** -- Lexer --
*/

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
	size_t			index;
	size_t			len;
	size_t			cap;
	t_lexer_state	state;
	t_oken			*tokens;
}						t_lexer;

typedef int				(*t_lexer_algo_fn)(t_lexer *,
	uint8_t *, t_oken_char[2], t_oken *);

void					init_token(t_oken *tok);
void					init_lexer(t_lexer *lexer);
bool					append_token(t_lexer *this, t_oken token);
bool					destroy_lexer(const t_lexer *lexer);

bool					expand_tildes(t_string *token, t_env *env);
bool					expand_dollars(t_string *token, t_env *env);

t_lexer					sh_lexer(t_string *string, t_env *env);
bool					destroy_lexer(const t_lexer *lexer);

/*
** -- AST --
*/

typedef struct			s_ast_node
{
	enum	{
		ROOT,
		COMMAND,
		ARG
	}		tag;
	union	{
		struct	{
			size_t				len;
			size_t				cap;
			struct s_ast_node	*commands;
		}		root;
		struct	{
			t_string	string;
			size_t		len;
			size_t		cap;
			t_string	*args;
			bool		dirty;
		}		command;
	}		payload;
}						t_ast_node;

void					init_ast_root(t_ast_node *root);
bool					init_ast_command(t_ast_node *command, t_string *string);
bool					destroy_ast(t_ast_node root, const t_lexer *lexer);

t_ast_node				sh_construct_ast(const t_lexer *lexer);

void					print_ast(t_ast_node root);

/*
** -- Execution --
*/

typedef enum			e_lookup_result
{
	LK_NOT_FOUND,
	LK_BUILTIN,
	LK_FOUND,
	LK_NO_RIGHTS,
	LK_PATH_TOO_LONG,
}						t_lookup_result;

int						sh_exec(t_string *string, t_env *env);

/*
** -- Prompt --
*/

typedef enum			e_color
{
	BLUE,
	RED,
	ORANGE
}						t_color;

char					*sh_prompt(size_t *prompt_len);

/*
** -- Autocompletion --
*/

enum					e_autocomplete_state
{
	AC_COMMAND,
	AC_FILE
};

typedef struct			s_completion
{
	enum e_autocomplete_state	state;
	size_t						len;
	char						*start;
}						t_completion;

bool					sh_autocomplete(char c, t_readline *rl,
	t_string *line);
char					*sh_complete_command(char *start, size_t len,
	char path[PATH_MAX], t_env *env);
char					*sh_complete_filename(char *start, size_t len,
	char path[PATH_MAX], bool must_exec);

t_lookup_result			sh_search_command(t_string *name, t_env *env,
	char path[PATH_MAX]);

/*
**  -- Shell builtins : mandatory and optional (bonus) --
*/

ssize_t					is_builtin(const char *name);

typedef int				(*t_shell_builtin_fn)(t_string *, size_t, t_env *);

typedef struct			s_shell_builtin
{
	char				*name;
	t_shell_builtin_fn	fn;
}						t_shell_builtin;

int						sh_builtin_env(t_string *args, size_t len,
	t_env *env);
int						sh_builtin_setenv(t_string *args, size_t len,
	t_env *env);
int						sh_builtin_unsetenv(t_string *args, size_t len,
	t_env *env);
int						sh_builtin_echo(t_string *args, size_t len,
	t_env *env);
int						sh_builtin_cd(t_string *args, size_t len,
	t_env *env);
int						sh_builtin_which(t_string *args, size_t len,
	t_env *env);

/*
** -- Global variables : necessary --
*/

extern t_env			*g_env;
extern t_shell_builtin	g_sh_builtins[];
extern pid_t			g_child_pid;
extern char				g_shell_pid[];
extern size_t			g_shell_pid_len;

#endif
