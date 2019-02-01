/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 09:34:03 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/01 16:35:36 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <stdio.h>

bool	sh_exec_ast(t_ast_node *root)
{
	(void)root;
	print_ast(*root);
	return (true);
}

void	print_lexer(t_lexer lexer)
{
	size_t  i;

	i = 0;
	while (i < lexer.len)
	{
		t_oken tok = lexer.tokens[i++];
		printf("{\n\ttype = %u\n\tstring = {\n\t\tlen = %zu\n\t\tstr = \"%s\"\n\t}\n}\n", tok.type, tok.payload.len, tok.payload.buff);
	}
}

int		sh_exec(t_string *string)
{
	const t_lexer	lexer = sh_lexer(string);
	t_ast_node		root;

	if (lexer.state != GLOBAL_SCOPE)
		return (ft_putf("syntax error\n"), 0);
	//print_lexer(lexer);
	root = sh_construct_ast(&lexer);
	sh_exec_ast(&root);
	return (0);
}
