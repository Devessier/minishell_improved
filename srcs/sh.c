/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 17:09:14 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/10 22:11:51 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

struct termios	g_original_termios;

void	sh_reset_raw_mode(void)
{
	tcsetattr(0, TCSAFLUSH, &g_original_termios);
}

void	sh_setup_raw_mode(void)
{
	struct termios	raw_termios;

	tcgetattr(0, &g_original_termios);
	raw_termios = g_original_termios;
	raw_termios.c_iflag &= ~(ICRNL | IXON);
	raw_termios.c_oflag &= ~(OPOST);
	raw_termios.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw_termios.c_cc[VMIN] = 0;
	raw_termios.c_cc[VTIME] = 1;
	tcsetattr(0, TCSAFLUSH, &raw_termios);
	atexit(sh_reset_raw_mode);
}

int		main(int argc, char **argv, char **envp)
{
	(void)argc, (void)argv, (void)envp;
	char	buffer[4096];
	size_t	bytes;
	size_t	total;

	sh_setup_raw_mode();
	total = 0;
	while (42)
	{
		if ((bytes = read(0, buffer, 4096)) > 0)
		{
			total += bytes;
			printf("bytes = %zu\n", bytes);
			if (*buffer == 'q')
				break ;
			write(1, buffer, 1);
		}
	}
	printf("total = %zu\n", total);
}
