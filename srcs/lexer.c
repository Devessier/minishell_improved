/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 09:30:25 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/17 18:08:33 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

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

size_t						append_token_env_var(t_oken *tok, char *str)
{
	size_t	i;
	char	*env_value;

	++str;
	if (*str == '$')
	{
		env_value = ft_itoa(getpid());
		concat_strings(&tok->payload, env_value, ft_strlen(env_value));
		free(env_value);
		return (1);
	}
	if (!(*str == '_' || ft_isalpha(*str)))
		return (0);
	i = 0;
	while (i[str] && (ft_isalnum(i[str]) || i[str] == '_'))
		i++;
	if (i > 0)
	{
		env_value = sh_get_env(str, i);
		concat_strings(&tok->payload, env_value, ft_strlen(env_value));
	}
	return (i);
}

bool						append_token(t_lexer *this, size_t index, t_oken token)
{
	t_oken	*tmp;
	size_t	cap;
	size_t	i;

	cap = this->cap == 0 ? 2 : this->cap;
	if (index + 1 > this->cap)
	{
		cap <<= 1;
		tmp = this->tokens;
		if (!(this->tokens = malloc(sizeof(t_oken) * cap)))
			return (false);
		i = 0;
		while (i++ < this->len)
			this->tokens[i - 1] = tmp[i - 1];
		free(tmp);
	}
	this->tokens[this->len++] = token;
	return (true);
}

bool						lexer_algorithm(t_lexer *lexer, uint8_t *str)
{
	t_oken_char	types[2];
	size_t		tmp;
	size_t		i;
	t_oken		tok;

	ft_bzero(types, 2 * sizeof(t_oken_char));
	init_lexer(lexer);
	init_token(&tok);
	i = 0;
	while (*str != '\0')
	{
		*types = g_tokens[*str];
		if (*str == '#')
			break ;
		if (*str == '$' && lexer->state != IN_SQUOTE)
		{
			tmp = append_token_env_var(&tok, (char *)str);
			if (tmp > 0)
				str += tmp;
			else if (!concat_strings(&tok.payload, "$", 1))
				return (false);
		}
		else if (lexer->state == GLOBAL_SCOPE)
		{
			if (types[1] == *types && (*types == T_AMPERSAND || *types == T_PIPE))
			{
				ft_putf("ampersand\n");
				if (!concat_strings(&tok.payload, (char *)str, 1))
					return (false);
			}
			if (*types == T_DQUOTE || *types == T_SQUOTE)
				lexer->state = *types == T_DQUOTE ? IN_DQUOTE : IN_SQUOTE;
			else if (*types == T_ESCAPE && str[1] != '\0' && !concat_strings(&tok.payload, (char *)++str, 1))
				return (false);
			else if (*types == T_WHITESPACE)
			{
				if (!append_token(lexer, i++, tok))
					return (false);
				// Don't forget to free the previous string
				init_token(&tok);
			}
			else if (!concat_strings(&tok.payload, (char *)str, 1))
				return (false);
		}
		else
		{
			if ((uint8_t)*types == (uint8_t)lexer->state)
				lexer->state = GLOBAL_SCOPE;
			else if (*types == T_ESCAPE && lexer->state == IN_DQUOTE && str[1] != '\0' && (str[1] == '$' || str[1] == '`' || str[1] == '\"' || str[1] == '\\' || str[1] == '\n'))
			{
				if (!(concat_strings(&tok.payload, (char *)++str, 1)))
					return (false);
			}
			else if (!concat_strings(&tok.payload, (char *)str, 1))
				return (false);
		}
		types[1] = *types;
		str++;
	}
	if (tok.payload.len > 0)
		append_token(lexer, i, tok);
	return (true);
}

t_lexer						sh_lexer(t_command *cmd)
{
	t_lexer			lexer;

	lexer_algorithm(&lexer, (uint8_t *)cmd->string.str);
	return (lexer);
}
