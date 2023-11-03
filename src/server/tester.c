#include <stdio.h>

int	main( void )
{
	fork();
	for (int i = 0; i < 2000; i++)
		system("curl 127.0.0.1:8090");
	wait(0);
	return (0);
}
