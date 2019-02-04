#include <stdio.h>

int		main(int argc, char **argv)
{
	while (argc--)
		puts(*argv++);
}
