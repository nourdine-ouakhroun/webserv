#include <iostream>
#include "Location.hpp"

int	main(int ac, char **av)
{
	if (ac < 2)
	{
		std::cerr << "Error :\fInvalid argument" << std::endl;
		return (1);
	}
	Location local;
	(void)local;
	(void)av;
	return (0);
}
