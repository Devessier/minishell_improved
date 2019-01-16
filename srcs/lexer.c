/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 09:30:25 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/16 13:24:22 by bdevessi         ###   ########.fr       */
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

bool						init_lexer(t_lexer **lexer)
{
	if (!(*lexer = malloc(sizeof(t_lexer))))
		return (false);
	(*lexer)->len = 0;
	(*lexer)->state = GLOBAL_SCOPE;
	return (true);
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

bool						append_token(t_lexer *lexer, char *str, size_t len, t_oken_char token_c)
{
	t_oken	tok;
	t_lexer	*tmp;
	size_t	i;

	tok = (t_oken) { .payload = { 0, 0, NULL }, .type = token_c };
	if (str != NULL)
	{
		if (!concat_strings(&tok.payload, str, len))
			return (false);
		ft_putf("payload = |%s|\n", tok.payload);
	}
	if (!(tmp = malloc(sizeof(t_lexer) + sizeof(t_oken) * (lexer->len + 1))))
	{
		if (tok.payload.str != NULL)
			free(tok.payload.str);
		return (false);
	}
	i = 0;
	tmp->len = lexer->len + 1;
	tmp->state = lexer->state;
	while (i++ < lexer->len)
		tmp->tokens[i - 1] = lexer->tokens[i - 1];
	free(lexer);
	return (true);
}

t_lexer_return				sh_lexer(t_command *cmd)
{
	t_lexer			*lexer;
	uint8_t			*tmp;
	t_oken_char		toks[2];
	char			*pointer;

	lexer = NULL;
	if (!init_lexer(&lexer))
		return (L_MALLOC_ERROR);
	tmp = (uint8_t *)cmd->string.str;
	while (*tmp != '\0')
	{
		*toks = g_tokens[*tmp];
		if (*toks == T_DQUOTE || *toks == T_SQUOTE)
		{
			lexer->state = *toks == T_DQUOTE ? IN_DQUOTE : IN_SQUOTE;
			pointer = ft_strchr((char *)tmp + 1, *tmp);
			if (pointer == NULL)
				return (L_SYNTAX_ERROR);
			append_token(lexer, (char *)tmp, pointer - (char *)tmp - 1, *toks);
		}
		toks[1] = *toks;
		tmp++;
	}
	return (lexer->state == GLOBAL_SCOPE ? L_PERFECT : L_SYNTAX_ERROR);
}
