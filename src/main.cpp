#include <fstream>
#include "parser/Location.hpp"
#include "parser/Server.hpp"
#include "Models/ServerModel.hpp"
#include <cctype>

std::vector<ServerModel> servers;

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

void	printLocations(Location* locs)
{
	if (!locs)
		return ;
	std::cout << "Path : "<< locs->getPath() << std::endl;
	std::vector<Data> vec = locs->getAllData();
	std::vector<Data>::iterator iBegin = vec.begin();
	std::vector<Data>::iterator iEnd = vec.end();
	while (iBegin != iEnd)
	{
		std::cout << "Key = " << iBegin->getKey() << "  Value = " << iBegin->getValue() << std::endl;
		iBegin++;
	}
	std::cout << "=================================================\n";
	std::vector<Location*> *innerLoc = locs->getInnerLocation();
	std::vector<Location*>::iterator b = innerLoc->begin();
	std::vector<Location*>::iterator e = innerLoc->end();
	while (b < e)
	{
		printLocations(*b);
		b++;
	}
}

Location*	getLocations(std::vector<String>::iterator& begin, const std::vector<String>::iterator& end, String	path)
{
	std::vector<Location*>* newLocation = new std::vector<Location*>;
	GlobalModel model;
	while (begin < end)
	{
		if (!begin->compare(0, 1, "}"))
		{
			begin++;
			break ;
		}
		if (!begin->compare(0, 9, "location "))
		{
			String _path = *begin;
			newLocation->push_back(getLocations(++begin, end, _path));
		}
		else
		{
			model.addData(extractDataFromString(*begin));
			begin++;
		}
	}
	return (new Location(model, path, newLocation));
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

void	parsingFile(std::vector<String> content)
{
	ServerModel server;
	std::vector<String>::iterator iBegin = content.begin();
	std::vector<String>::iterator iEnd = content.end();
	while (iBegin < iEnd)
	{
		if (iBegin->compare(0, 9, "location "))
			server.addData(extractDataFromString(*iBegin++));
		else
			server.addLocation(getLocations(++iBegin, iEnd, *(iBegin - 1)));
	}
	servers.push_back(server);
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


void	freeLocations(Location* locs)
{
	if (!locs)
		return ;
	std::vector<Location*> *innerLoc = locs->getInnerLocation();
	std::vector<Location*>::iterator b = innerLoc->begin();
	std::vector<Location*>::iterator e = innerLoc->end();
	while (b < e)
	{
		freeLocations(*b);
		delete *b;
		*b = NULL;
		b++;
	}
}

void	printServerModel(ServerModel& server)
{
	std::vector<Location*>::const_iterator b = server.getLocation().cbegin();
	std::vector<Location*>::const_iterator e = server.getLocation().cend();
	while (b < e)
	{
		printLocations(*b);
		freeLocations(*b);
		b++;
	}
}

void	testLeaks(char *fileName)
{
	std::vector<String> content;
	try
	{
		content = getFileContent(fileName);
	}
	catch (std::exception& e)
	{
		std::cerr << "Error :\fcan't open the file " << fileName << " please try again with deffrent file Name." << std::endl;
		return ;
	}
//	checkSyntax(content);
	std::vector<std::vector<String> > sers = splitContentIntoServers(content);
	std::vector<std::vector<String> >::iterator begin = sers.begin();
	std::vector<std::vector<String> >::iterator end = sers.end();
	while (begin < end)
	{
		parsingFile(*begin);
		begin++;
	}
//	std::vector<Data> data = server.begin()->getAllData();
	std::vector<ServerModel>::iterator b = servers.begin();
	std::vector<ServerModel>::iterator e = servers.end();
	int i = 0;
	while (b < e)
	{
		std::cout << "===================================== Server " << i++ << "====================================" << std::endl;
		printServerModel(*b);
		b++;
	}
	//delete server;
}

int	main(int ac, char **av)
{
	if (ac < 2)
	{
		std::cerr << "Error :\fInvalid argument" << std::endl;
		return (1);
	}
	testLeaks(av[1]);
	system("leaks -q webServ");	
	//checkSyntax(data);

	return (0);
}
