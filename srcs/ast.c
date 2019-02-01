/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/31 16:54:34 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/01 18:08:22 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include <stdbool.h>

void		init_ast_root(t_ast_node *root)
{
	*root = (t_ast_node) {
		.tag = ROOT,
		.payload = {
			.root = {
				.cap = 0,
				.len = 0,
				.commands = NULL
			}
		}
	};
}

bool		init_ast_command(t_ast_node *command, t_string *string)
{
	*command = (t_ast_node) {
		.tag = COMMAND,
		.payload = {
			.command = {
				.string = { 0, 0, NULL },
				.args = { 0, 0, NULL },
				.dirty = false,
			},
		},
	};
	if (string != NULL)
		return ((command->payload.command.string = ft_new_string(string->buff, false)).buff != NULL);
	return (true);
}

bool		append_command_to_root(t_ast_node *root, t_ast_node *command)
{
	t_ast_node	*commands;
	size_t		i;
	size_t		cap;

	cap = root->payload.root.cap == 0 ? 1 : root->payload.root.cap;
	if (root->payload.root.len + 1 > root->payload.root.cap)
	{
		cap <<= 1;
		if (!(commands = malloc(sizeof(t_ast_node) * cap)))
			return (false);
		i = 0;
		while (i++ < root->payload.root.len)
			commands[i - 1] = root->payload.root.commands[i - 1];
		free(root->payload.root.commands);
		root->payload.root.commands = commands;
		root->payload.root.cap = cap;
	}
	root->payload.root.commands[root->payload.root.len++] = *command;
	command->payload.command.dirty = true;
	return (true);
}

void		print_ast(t_ast_node root)
{
	size_t		i = 0;
	while (i < root.payload.root.len)
	{
		ft_putf("command = %s", root.payload.root.commands[i].payload.command.string.buff);
		if (root.payload.root.commands[i].payload.command.args.buff)
			ft_putf("; args = %s\n", root.payload.root.commands[i].payload.command.args.buff);
		else
			ft_putchar('\n');
		i++;
	}
}

t_ast_node	sh_construct_ast(const t_lexer *lexer)
{
	t_ast_node	root;
	t_ast_node	command;
	ssize_t		i;
	size_t		args_index;

	init_ast_root(&root);
	init_ast_command(&command, NULL);
	i = -1;
	args_index = 0;
	while (++i < (ssize_t)lexer->len)
	{
		if (lexer->tokens[i].type != T_SEMICOLON)
		{
			if (args_index == 0)
				init_ast_command(&command, &lexer->tokens[i].payload);
			else
			{
				if (command.payload.command.args.len > 0)
					ft_concat_strings(&command.payload.command.args, " ", 1);
				ft_concat_strings(&command.payload.command.args, lexer->tokens[i].payload.buff, lexer->tokens[i].payload.len);	
			}
			args_index++;
		}
		else if (command.payload.command.string.len > 0 && !command.payload.command.dirty)
		{
			append_command_to_root(&root, &command);
			args_index = 0;
		}
	}
	if (command.payload.command.string.len > 0 && !command.payload.command.dirty)
		append_command_to_root(&root, &command);
	destroy_lexer(lexer);
	return (root);
}
