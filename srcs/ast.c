/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/31 16:54:34 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/05 15:20:23 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include <stdbool.h>

static bool	append_command_to_root(t_ast_node *root, t_ast_node *command)
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

static bool	append_arg_to_root(t_ast_node *command, t_string *arg)
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

static bool	ast_loop(const t_lexer *lexer, t_ast_node *root,
	t_ast_node *command)
{
	ssize_t	i;
	size_t	args_index;

	i = -1;
	args_index = 0;
	while (++i < (ssize_t)lexer->len)
		if (lexer->tokens[i].type != T_SEMICOLON)
		{
			if (args_index == 0)
			{
				if (!init_ast_command(command, &lexer->tokens[i].payload))
					return (false);
			}
			else if (!append_arg_to_root(command, &lexer->tokens[i].payload))
				return (false);
			args_index++;
		}
		else if ((*command).payload.command.string.len > 0
				&& !(*command).payload.command.dirty)
		{
			if (!append_command_to_root(root, command))
				return (false);
			args_index = 0;
		}
	return (true);
}

t_ast_node	sh_construct_ast(const t_lexer *lexer)
{
	t_ast_node	root;
	t_ast_node	command;

	init_ast_root(&root);
	init_ast_command(&command, NULL);
	if (!ast_loop(lexer, &root, &command))
	{
		destroy_ast(root, lexer);
		return ((t_ast_node) { .tag = ROOT });
	}
	if (command.payload.command.string.len > 0
			&& !command.payload.command.dirty)
	{
		if (!append_command_to_root(&root, &command))
			return ((t_ast_node) { .tag = ROOT });
	}
	return (root);
}
