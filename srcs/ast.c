/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/31 16:54:34 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/31 17:52:02 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void		init_ast_root(t_ast_node *root)
{
	*root = (t_ast_node) {
		.tag = ROOT,
		.payload = {
			.root = {
				.len = 0,
				.commands = NULL
			}
		}
	};
}

void		init_ast_command(t_ast_node *command, t_string string)
{
	*command = (t_ast_node) {
		.tag = COMMAND,
		.payload = {
			.command = {
				.string = string,
				.len = 0,
				.args = NULL
			}
		}
	};
}

bool		append_command_to_root() ;

t_ast_node	*make_ast_arg(t_string string)
{
	t_ast_node	*node;

	if (!(node = malloc(sizeof(t_ast_node))))
		return (NULL);
	node->tag = ARG;
	node->payload.argument = string;
	return (node);
}

t_ast_node	*make_ast_command() ;

t_ast_node	construct_ast(t_lexer *lexer)
{
	t_ast_node	root;
	t_ast_node	command;
	size_t		i;
	size_t		args_index;

	init_ast_root(&root);
	i = 0;
	args_index = 0;
	while (i < lexer->len)
	{
		if (args_index == 0)
			init_ast_command(&command, lexer->tokens[root.payload.root.len].payload);
		// append args to the node if there are some one
		if (lexer->tokens[i].type == T_SEMICOLON)
		{
			// "close" current command, "open" a new one

		}
	}
}
