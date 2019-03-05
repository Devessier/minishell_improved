/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 09:30:25 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/01 18:57:56 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/syslimits.h>

static const t_oken_char	g_tokens[1 << 7] = {
	['\"'] = T_DQUOTE,
	['\''] = T_SQUOTE,
	[' '] = T_WHITESPACE,
	[';'] = T_SEMICOLON,
	['\\'] = T_ESCAPE,
	['&'] = T_AMPERSAND,
	['|'] = T_PIPE,
};

static bool					whitespace_semicolon(t_lexer *lexer,
	uint8_t *str, t_oken_char types[2], t_oken *tok)
{
	if (tok->payload.len > 0 && !append_token(lexer, *tok))
		return (false);
	if (*types == T_SEMICOLON)
	{
		if (str[1] == ';')
		{
			lexer->state = EXPLICIT_SYNTAX_ERROR;
			return (false);
		}
		init_token(tok);
		tok->type = T_SEMICOLON;
		if (!append_token(lexer, *tok))
			return (false);
	}
	ft_bzero(types, 2 * sizeof(t_oken_char));
	init_token(tok);
	return (true);
}

static int					global_scope(t_lexer *lexer,
	uint8_t *str, t_oken_char types[2], t_oken *tok)
{
	const uint8_t	*start = str;

	if (*types == T_ESCAPE)
	{
		if (*str != '\0' && !ft_concat_strings(&tok->payload, (char *)++str, 1))
			return (-1);
	}
	else if (*types == T_DQUOTE || *types == T_SQUOTE)
	{
		tok->type = *types;
		lexer->state = *types == T_DQUOTE ? IN_DQUOTE : IN_SQUOTE;
	}
	else if (*types == T_WHITESPACE || *types == T_SEMICOLON)
	{
		if (!whitespace_semicolon(lexer, str, types, tok))
			return (-1);
	}
	else
	{
		tok->type = T_WORD;
		if (!ft_concat_strings(&tok->payload, (char *)str, 1))
			return (-1);
	}
	return (str - start);
}

static int					not_global_scope(t_lexer *lexer, uint8_t *str,
	t_oken_char types[2], t_oken *tok)
{
	const uint8_t	*start = str;

	if ((uint8_t)*types == (uint8_t)lexer->state)
		lexer->state = GLOBAL_SCOPE;
	else if (*types == T_ESCAPE && lexer->state == IN_DQUOTE
		&& str[1] != '\0' && (str[1] == '$' || str[1] == '`'
		|| str[1] == '\"' || str[1] == '\\' || str[1] == '\n'))
	{
		if (!(ft_concat_strings(&tok->payload, (char *)++str, 1)))
			return (-1);
	}
	else if (!ft_concat_strings(&tok->payload, (char *)str, 1))
		return (-1);
	return (str - start);
}

static bool					append_guard(t_lexer *lexer, t_oken *tok)
{
	if (!append_token(lexer, *tok))
	{
		ft_free_string(&tok->payload);
		return (false);
	}
	return (true);
}

bool						lexer_algorithm(t_lexer *lexer, uint8_t *str)
{
	t_lexer_algo_fn	lexer_fn;
	t_oken_char		types[2];
	t_oken			tok;
	int				move;

	ft_bzero(types, 2 * sizeof(t_oken_char));
	init_token(&tok);
	while (str && !(*str == '\0' || *str == '#'))
	{
		*types = g_tokens[*str];
		lexer_fn = lexer->state == GLOBAL_SCOPE
			? global_scope : not_global_scope;
		if ((move = lexer_fn(lexer, str, types, &tok)) == -1)
			return (!ft_free_string(&tok.payload));
		else
			str += move;
		types[1] = *types;
		str++;
	}
	if (tok.payload.len > 0)
	{
		tok.type = *types;
		return (append_guard(lexer, &tok));
	}
	return (true);
}

t_lexer						sh_lexer(t_string *string, t_env *env)
{
	t_lexer	lexer;
	size_t	i;

	init_lexer(&lexer);
	if (!lexer_algorithm(&lexer, (uint8_t *)string->buff))
		return ((t_lexer) { 0, 0, 0, 0, NULL });
	if (*g_shell_pid == '\0')
		ft_itoa_buff(getpid(), g_shell_pid);
	g_shell_pid_len = ft_strlen(g_shell_pid);
	i = 0;
	while (i++ < lexer.len)
	{
		if (lexer.tokens[i - 1].type == T_WORD
				&& *lexer.tokens[i - 1].payload.buff == '~')
			if (!expand_tildes(&lexer.tokens[i - 1].payload, env))
				return ((t_lexer) { 0, 0, 0, 0, NULL });
		if (lexer.tokens[i - 1].type != T_SQUOTE
				&& !expand_dollars(&lexer.tokens[i - 1].payload, env))
			return ((t_lexer) { 0, 0, 0, 0, NULL });
	}
	return (lexer);
}
