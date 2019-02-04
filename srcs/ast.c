/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/31 16:54:34 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/04 15:18:45 by bdevessi         ###   ########.fr       */
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
				.len = 0,
				.cap = 0,
				.args = NULL,
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

bool		append_arg_to_root(t_ast_node *command, t_string *arg)
{
	t_string	*args;
	size_t		cap;
	size_t		i;

	cap = command->payload.command.cap == 0 ? 1 : command->payload.command.cap;
	if (command->payload.command.len + 1 > command->payload.command.cap)
	{
		cap <<= 1;
		if (!(args = malloc(sizeof(t_string) * cap)))
			return (false);
		i = 0;
		while (i++ < command->payload.command.len)
			args[i - 1] = command->payload.command.args[i - 1];
		free(command->payload.command.args);
		command->payload.command.args = args;
		command->payload.command.cap = cap;
	}
	command->payload.command.args[command->payload.command.len++] = *arg;
	return (true);
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
			else if (!append_arg_to_root(&command, &lexer->tokens[i].payload))
				; // malloc error
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
	return (root);
}
