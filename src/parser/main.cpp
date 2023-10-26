#include <iostream>
#include "Location.hpp"
#include "Server.hpp"
#include "../Models/GlobalModel.hpp"

int	main(int ac, char **av)
{
	if (ac < 2)
	{
		std::cerr << "Error :\fInvalid argument" << std::endl;
		return (1);
	}
	GlobalModel model("80", "./YoupiBanane", "index.html", "GET", "hello");
	std::cout << model.getListen() << std::endl;
	std::cout << model.getRoot() << std::endl;
	std::cout << model.getIndex() << std::endl;
	std::cout << model.getAllowMethods() << std::endl;
	std::cout << model.getErrorPage() << std::endl;
	Server ser();
/*	Server serv;
	serv.pushToMap("listen", "80");
	serv.pushToMap("listen", "[::]:80");
	serv.pushToMap("listen", "441");
	Location local;
	serv.pushToVector(local);
	serv.displayMapContent();*/
	(void)av;
	return (0);
}
