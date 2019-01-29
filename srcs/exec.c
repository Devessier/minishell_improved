/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 09:34:03 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/29 17:37:41 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <stdio.h>

void	print_lexer(t_lexer lexer)
{
	size_t	i;

	i = 0;
	ft_putchar('\n');
	while (i < lexer.len)
	{
		t_oken tok = lexer.tokens[i++];
		printf("{\n\ttype = %u\n\tstring = {\n\t\tlen = %zu\n\t\tstr = \"%s\"\n\t}\n}\n", tok.type, tok.payload.len, tok.payload.buff);
	}
}

int		sh_exec(t_string *string)
{
	const t_lexer	lexer = sh_lexer(string);

	print_lexer(lexer);
	if (lexer.state != GLOBAL_SCOPE)
		ft_putf("syntax error\n");
	return (0);
}
