#include <stdio.h>

int	main( void )
{
	for (int i = 0; i < 2000; i++)
		system("curl 169.254.150.124:8090");
	return (0);
}
