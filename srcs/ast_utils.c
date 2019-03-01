/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 15:42:58 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/01 16:03:26 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	init_ast_root(t_ast_node *root)
{
	*root = (t_ast_node) {
		.tag = ROOT,
		.payload = {
			.root = {
				.cap = 0,
				.len = 0,
				.commands = NULL,
			}
		}
	};
}

bool	init_ast_command(t_ast_node *command, t_string *string)
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
	{
		command->payload.command.string = ft_new_string(string->buff, false);
		return (command->payload.command.string.buff != NULL);
	}
	return (true);
}

bool	destroy_ast(t_ast_node root, const t_lexer *lexer)
{
	size_t		i;
	t_ast_node	el;

	i = 0;
	while (i < root.payload.root.len)
	{
		el = root.payload.root.commands[i];
		ft_free_string(&el.payload.command.string);
		free(el.payload.command.args);
		i++;
	}
	free(root.payload.root.commands);
	return (destroy_lexer(lexer));
}
