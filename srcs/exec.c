/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 09:34:03 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/03 23:19:11 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <stdio.h>

static const t_shell_builtin	sh_builtins[] = {
	{ "exit", sh_builtin_exit },
	{ "env", sh_builtin_env },
	{ "setenv", sh_builtin_setenv },
	{ "unsetenv", sh_builtin_unsetenv },
	{ "echo", sh_builtin_echo },
	{ "cd", sh_builtin_cd },
	{ "which", sh_builtin_which },
};


int								sh_exec(t_string *string)
{
	(void)sh_builtins;
	const t_lexer	lexer = sh_lexer(string);
	t_ast_node		root;

	if (lexer.state != GLOBAL_SCOPE)
		return (ft_putf("syntax error\n"), 0);
	root = sh_construct_ast(&lexer);
	print_ast(root);
	return (0);
}
