/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 09:30:25 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/28 16:48:41 by bdevessi         ###   ########.fr       */
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


void						init_token(t_oken *tok)
{
	*tok = (t_oken) {
		.payload = { 0, 0, NULL },
		.type = T_WORD,
	};
}

void						init_lexer(t_lexer *lexer)
{
	*lexer = (t_lexer) {
		.len = 0,
		.cap = 0,
		.state = GLOBAL_SCOPE,
		.tokens = NULL
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

bool						append_token(t_lexer *this, size_t index, t_oken token)
{
	t_oken	*tmp;
	size_t	cap;
	size_t	i;

	cap = this->cap == 0 ? 1 : this->cap;
	if (index + 1 > this->cap)
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
	this->tokens[index] = token;
	return (true);
}

bool						lexer_algorithm(t_lexer *lexer, uint8_t *str)
{
	t_oken_char	types[2];
	size_t		i;
	t_oken		tok;

	ft_bzero(types, 2 * sizeof(t_oken_char));
	init_token(&tok);
	i = 0;
	while (str && *str != '\0')
	{
		*types = g_tokens[*str];
		if (*str == '#')
			break ;
		if (lexer->state == GLOBAL_SCOPE)
		{
			if (*types == T_ESCAPE)
			{
				if (*str != '\0' && !ft_concat_strings(&tok.payload, (char *)++str, 1))
					return (false);
			}
			else if (*types == T_DQUOTE || *types == T_SQUOTE)
			{
				tok.type = *types;
				lexer->state = *types == T_DQUOTE ? IN_DQUOTE : IN_SQUOTE;
			}
			else if (*types == T_WHITESPACE || *types == T_SEMICOLON)
			{
				if (tok.payload.len > 0 && !append_token(lexer, i++, tok))
					return (false);
				if (*types == T_SEMICOLON)
				{
					if (str[1] == ';')
					{
						// free
						return (lexer->state = EXPLICIT_SYNTAX_ERROR, false);
					}
					init_token(&tok);
					tok.type = T_SEMICOLON;
					if (!append_token(lexer, i++, tok))
						return (false);
				}
				// Don't forget to free the previous string
				ft_bzero(types, 2 * sizeof(t_oken_char));
				init_token(&tok);
			}
			else
			{
				tok.type = T_WORD;
				if (!ft_concat_strings(&tok.payload, (char *)str, 1))
					return (false);
			}
		}
		else
		{
			if ((uint8_t)*types == (uint8_t)lexer->state)
				lexer->state = GLOBAL_SCOPE;
			else if (*types == T_ESCAPE && lexer->state == IN_DQUOTE && str[1] != '\0' && (str[1] == '$' || str[1] == '`' || str[1] == '\"' || str[1] == '\\' || str[1] == '\n'))
			{
				if (!(ft_concat_strings(&tok.payload, (char *)++str, 1)))
					return (false);
			}
			else if (!ft_concat_strings(&tok.payload, (char *)str, 1))
				return (false);
		}
		types[1] = *types;
		str++;
	}
	if (tok.payload.len > 0)
	{
		tok.type = *types;
		append_token(lexer, i, tok);
	}
	return (true);
}



bool						destroy_lexer(const t_lexer *lexer)
{
	size_t	i;

	i = 0;
	while (i < lexer->len)
		free(lexer->tokens[i++].payload.buff);
	free(lexer->tokens);
	return (true);
}

t_lexer						sh_lexer(t_string *string, t_env *env)
{
	t_lexer			lexer;
	size_t		i;	

	init_lexer(&lexer);
	if (!lexer_algorithm(&lexer, (uint8_t *)string->buff))
		; //error
	if (*g_shell_pid == '\0')
		ft_itoa_buff(getpid(), g_shell_pid);
	g_shell_pid_len = ft_strlen(g_shell_pid);
	i = 0;
	while (i++ < lexer.len)
	{
		if (lexer.tokens[i - 1].type == T_WORD && *lexer.tokens[i - 1].payload.buff == '~')
			if (!expand_tildes(&lexer.tokens[i - 1].payload, env))
				; // error
		if (lexer.tokens[i - 1].type != T_SQUOTE && !expand_dollars(&lexer.tokens[i - 1].payload, env))
			; // error

	}
	return (lexer);
}
