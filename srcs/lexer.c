/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 09:30:25 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/15 16:43:01 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <stdbool.h>

static const t_oken_char	g_tokens[1 << 7] = {
	['\"'] = T_DQUOTE,
	['\''] = T_SQUOTE,
	[' '] = T_WHITESPACE,
	[';'] = T_SEMICOLON,
	['\\'] = T_ESCAPE,
};

void						init_lexer(t_lexer *lexer)
{
	*lexer = (t_lexer) {
		.len = 0,
		.state = GLOBAL_SCOPE
	};
}

void						copy_lexer(t_lexer *new, t_lexer *old)
{
	size_t	i;

	i = 0;
	new->len = old->len + 1;
	new->state = old->state;
	while (i++ < old->len)
		new->tokens[i - 1] = old->tokens[i - 1];
	free(new);
}

bool						append_token(t_lexer *lexer, size_t index, char c, t_oken_char token_c)
{
	t_lexer		*tmp;
	t_string	*string;

	if (index < lexer->len)
	{
		string = &lexer->tokens[index].string;
		if (!extend_string(string, 1))
			return (false);
		string->str[string->len++] = c;
	}
	else
	{
		if ((*string = new_string(&c, true)).cap > 0)
			return (false);
		// free underlying string
		if (!(tmp = malloc(sizeof(t_lexer) + sizeof(t_oken) * (lexer->len + 1))))
			return (false);
		copy_lexer(tmp, lexer);
		*lexer = tmp;
	}
	return (true);
}

bool						append_string(t_lexer *lexer, char *str, char delim)
{
	const char	*tmp = ft_strchr(str, delim);
	
	if (tmp == NULL)
		return (false);

}

bool						sh_lexer(t_command *cmd)
{
	const uint8_t	*start = (uint8_t *)cmd->string.str;
	t_lexer			*lexer;
	uint8_t			*tmp;
	t_oken_char		toks[2];

	init_lexer(lexer);
	tmp = start;
	while (*tmp != '\0')
	{
		*toks = g_tokens[*tmp];
		if (*toks == T_DQUOTE || *toks == T_SQUOTE)
		{
			lexer->state = *toks == T_DQUOTE ? IN_DQUOTE : IN_SQUOTE;
			while ()
			append_token(lexer, start - tmp, *tmp, *toks);
		}
		else if (*toks = T_SQUOTE)
		{
			lexer->state = IN_SQUOTE;
		}
		toks[1] = *toks;
		tmp++;
	}
	return (state == GLOBAL_SCOPE);
}
