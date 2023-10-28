#include <fstream>
#include "Location.hpp"
#include "Server.hpp"
#include "../Models/ServerModel.hpp"
#include <cctype>


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

std::vector<String>	getFileContent(String fileName)
{
	std::ifstream outfile(fileName);
	if (outfile.is_open() == false)
	{
		outfile.close();
		throw (std::exception());
	}
	std::vector<String>	vec;
	String tmp;
	while (!outfile.eof())
	{
		std::getline(outfile, tmp, '\n');
		tmp.trim(" \t");
		if (tmp.size() == 0 || *tmp.begin() == '#')
			continue ;
		vec.push_back(tmp);
	}
	outfile.close();
	return (vec);
}

Data	extractDataFromString(String& line)
{
	std::vector<String> vec = line.split(' ');
	std::vector<String>::iterator ib = vec.begin();
	std::vector<String>::iterator ie = vec.end();
	String	key = *ib;
	String	value;
	while (++ib < ie)
		value.append("\t").append(ib->trim(" \t"));
	return (Data(key, value));
}

std::vector<String>	getServerConfig(std::vector<String>::iterator& iterBegin, const std::vector<String>::iterator& iterEnd)
{
	int	openBrackets = 0;
	std::vector<String> server;
	bool	insideServer = false;

	while (iterBegin < iterEnd)
	{
		openBrackets += iterBegin->countRepeating('{');
		openBrackets -= iterBegin->countRepeating('}');
		if (!openBrackets)
		{
			iterBegin++;
			break ;
		}
		if (!iterBegin->compare(0, 7, "server "))
			insideServer = !insideServer;
		if (insideServer == true)
			server.push_back(iterBegin->trim(" \t"));
		iterBegin++;
	}
	return server;
}

/*
void	checker(std::vector<String> vec)
{
	std::vector<std::vector<String>> servers;
	
}*/

ServerModel	parsingFile(std::vector<String> content)
{
	ServerModel server;
	std::vector<String>::iterator iBegin = content.begin();
	std::vector<String>::iterator iEnd = content.end();
	while (++iBegin < iEnd)
	{
		if (iBegin->compare(0, 9, "location ") && iBegin->compare(0, 1, "}"))
			server.addData(extractDataFromString(*iBegin));
	}
	return (server);
}

int	checkSyntax(std::vector<Data> content)
{
	std::vector<Data>::iterator iterBegin = content.begin();
	std::vector<Data>::iterator iterEnd = content.end();
	while (iterBegin != iterEnd)
	{
		std::cout << iterBegin->getKey() << "\t\t\t\t" << iterBegin->getValue() << std::endl;
		iterBegin++;
	}
	return (1);
}

std::vector<std::vector<String> >	splitContentIntoServers(std::vector<String>& content)
{
	std::vector<std::vector<String> > servers;
	std::vector<String>::iterator begin = content.begin();
	const std::vector<String>::iterator end = content.end();
	while (begin < end)
		servers.push_back(getServerConfig(begin, end));
	return (servers);
}

int	main(int ac, char **av)
{
	if (ac < 2)
	{
		std::cerr << "Error :\fInvalid argument" << std::endl;
		return (1);
	}
	std::vector<String> content;
	try
	{
		content = getFileContent(av[1]);
	}
	catch (std::exception& e)
	{
		std::cerr << "Error :\fcan't open the file " << av[1] << " please try again with deffrent file Name." << std::endl;
		return (1);
	}
//	checkSyntax(content);
	std::vector<std::vector<String> > servers = splitContentIntoServers(content);
	ServerModel server = parsingFile(*servers.begin());
	std::vector<Data> data = server.getAllData();	
	
	checkSyntax(data);

	return (0);
}
