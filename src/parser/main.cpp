#include <iostream>
#include "Location.hpp"
#include "Server.hpp"
#include "../Models/GlobalModel.hpp"
#include "../Models/ServerModel.hpp"

Location*	getLocations(Location* originLocation, GlobalModel model)
{
	static int i;
	Location *newLocation = NULL;
	if (i++ < 5)
		newLocation = new Location(model, getLocations(originLocation, model));
	return (newLocation);
}

void	printLocations(Location* locs)
{
	if (!locs)
		return ;
	std::cout << locs->getListen() << std::endl;
	std::cout << locs->getRoot() << std::endl;
	std::cout << locs->getIndex() << std::endl;
	std::cout << locs->getAllowMethods() << std::endl;
	std::cout << locs->getErrorPage() << std::endl;
	std::cout << "=================================================\n";
	Location *innerLoc = locs->getInnerLocation();
	if (!innerLoc)
		return ;
	printLocations(innerLoc);
}

int	main(int ac, char **av)
{
	if (ac < 2)
	{
		std::cerr << "Error :\fInvalid argument" << std::endl;
		return (1);
	}
	GlobalModel model("80", "./YoupiBanane", "index.html", "GET", "hello");
/*	Location*	loc = new Location(
			GlobalModel("8080", "/mehdi", "home.html", "POST", "Salim"),
			new Location(
				GlobalModel("8080", "/mehdi", "home.html", "POST", "Salim"),
				NULL)
			);*/
	Location *loc = getLocations(new Location(model, NULL), model);
	printLocations(loc);
	std::cout << model.getListen() << std::endl;
	std::cout << model.getRoot() << std::endl;
	std::cout << model.getIndex() << std::endl;
	std::cout << model.getAllowMethods() << std::endl;
	std::cout << model.getErrorPage() << std::endl;
	ServerModel sermodel(model, "hello", "test", loc);
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
