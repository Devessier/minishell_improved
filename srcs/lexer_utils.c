/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 16:05:37 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/01 17:39:42 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	init_token(t_oken *tok)
{
	*tok = (t_oken) {
		.payload = { 0, 0, NULL },
		.type = T_WORD,
	};
}

void	init_lexer(t_lexer *lexer)
{
	*lexer = (t_lexer) {
		.index = 0,
		.len = 0,
		.cap = 0,
		.state = GLOBAL_SCOPE,
		.tokens = NULL
	};
}

bool	append_token(t_lexer *this, t_oken token)
{
	t_oken	*tmp;
	size_t	cap;
	size_t	i;

	cap = this->cap == 0 ? 1 : this->cap;
	if (this->index + 1 > this->cap)
	{
		cap <<= 1;
		tmp = this->tokens;
		if (!(this->tokens = malloc(sizeof(t_oken) * cap)))
			return (false);
		this->cap = cap;
		i = 0;
		while (i++ < this->len)
			this->tokens[i - 1] = tmp[i - 1];
		free(tmp);
	}
	this->len++;
	this->tokens[this->index++] = token;
	return (true);
}

bool	destroy_lexer(const t_lexer *lexer)
{
	size_t	i;

	i = 0;
	while (i < lexer->len)
		free(lexer->tokens[i++].payload.buff);
	free(lexer->tokens);
	return (true);
}
