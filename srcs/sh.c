/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 17:09:14 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/10 17:13:29 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

struct termios	g_original_termios;

void	sh_reset_raw_mode(void)
{
	tcsetattr(0, TCSAFLUSH, &g_original_termios);
}

void	sh_setup_raw_mode(void)
{
	struct termios	raw_termios;

	tcgetattr(0, &g_original_termios);
	atexit(sh_reset_raw_mode);
	raw_termios = g_original_termios;
	raw_termios.c_iflag &= ~(ICRNL | IXON);
	raw_termios.c_oflag &= ~(OPOST);
	raw_termios.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw_termios.c_cc[VMIN] = 0;
	raw_termios.c_cc[VTIME] = 1;
	tcsetattr(0, TCSAFLUSH, &raw_termios);
}

int		main(int argc, char **argv, char **envp)
{
	char	buffer[4096];
	size_t	bytes;

	sh_setup_raw_mode(void);
	while (42)
	{
		bytes = read(0, buffer, 1);
		if (*buffer = 'q')
			break ;
	}
}
