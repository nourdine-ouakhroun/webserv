#include <iostream>
#include <fstream>
#include <vector>
#include "Location.hpp"
#include "Server.hpp"
#include "../Models/GlobalModel.hpp"
#include "../Models/ServerModel.hpp"
#include "../Models/Data.hpp"

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
	std::vector<Data> vec = locs->getAllData();
	std::vector<Data>::iterator iBegin = vec.begin();
	std::vector<Data>::iterator iEnd = vec.end();
	while (iBegin != iEnd)
	{
		std::cout << "Key = " << iBegin->getKey() << "  Value = " << iBegin->getValue() << std::endl;
		iBegin++;
	}
	std::cout << "=================================================\n";
	Location *innerLoc = locs->getInnerLocation();
	if (!innerLoc)
		return ;
	printLocations(innerLoc);
}

std::vector<std::string>	getFileContent(std::string fileName)
{
	std::ifstream outfile(fileName);
	std::vector<std::string>	vec;
	std::string tmp;
	while (!outfile.eof())
	{
		std::getline(outfile, tmp, '\n');
		vec.push_back(tmp);
	}

	return (vec);
}

int	checkSyntax(std::vector<std::string> content)
{
	std::vector<std::string>::iterator iterBegin = content.begin();
	std::vector<std::string>::iterator iterEnd = content.end();
	while (iterBegin != iterEnd)
	{
		std::cout << *iterBegin << std::endl;
		iterBegin++;
	}
	return (1);
}

int	main(int ac, char **av)
{
	if (ac < 2)
	{
		std::cerr << "Error :\fInvalid argument" << std::endl;
		return (1);
	}

	std::vector<std::string> content = getFileContent(av[1]);
	checkSyntax(content);

	GlobalModel model;
	model.addData(Data("listen", "80"));
	model.addData(Data("listen", "[::]:80"));
	model.addData(Data("server_name", "mehdi salim"));
	Location *loc = getLocations(new Location(model, NULL), model);
	printLocations(loc);
	ServerModel sermodel(model, loc);
	std::cout << "============= Server Model =============" << std::endl;
	(void)av;
	return (0);
}
