#include <iostream>
#include "Location.hpp"
#include "Server.hpp"
#include "../Models/GlobalModel.hpp"
#include "../Models/ServerModel.hpp"

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
	ServerModel sermodel(model, "hello", "test", NULL);
	std::cout << "============= Server Model =============" << std::endl;
	std::cout << sermodel.getServerName() << std::endl;
	std::cout << sermodel.getAccessLog() << std::endl;
	std::cout << sermodel.getListen() << std::endl;
	std::cout << sermodel.getRoot() << std::endl;
	std::cout << sermodel.getIndex() << std::endl;
	std::cout << sermodel.getAllowMethods() << std::endl;
	std::cout << sermodel.getErrorPage() << std::endl;
//	Server ser();
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
