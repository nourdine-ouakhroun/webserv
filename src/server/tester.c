#include <stdio.h>

int	main( void )
{
	for (int i = 0; i < 1000; i++)
		system("curl -i -H \"Accept: application/json\" -H \"Content-Type: application/json\" -H \"Host: localhost.com\" 127.0.0.1:8080");
	return (0);
}
