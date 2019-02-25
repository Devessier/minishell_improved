/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 12:20:51 by bdevessi          #+#    #+#             */
/*   Updated: 2019/02/25 13:17:57 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include "readline.h"
#include <string.h>
#include <unistd.h>

char	*sh_prompt(size_t *prompt_len)
{
	static char	prompt[PATH_MAX + 36] = { 0 };
	char		*cwd;

	*prompt_len = ft_strlen(PROMPT_START);
	if ((cwd = getcwd(prompt + 34, PATH_MAX)) == NULL)
		return (ft_strcpy(prompt, PROMPT_START));
	ft_memcpy(prompt, CSI "38;5;42m" PROMPT_START COLOUR_RESET " [", 34);
	ft_strcat(prompt, "]");
	*prompt_len += 2 + ft_strlen(prompt + 34);
	return (prompt);
}
