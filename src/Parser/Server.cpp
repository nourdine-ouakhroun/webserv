#include "Server.hpp"

Server::Server( void )
{
}
Server::Server(const Server& copy)
{
	*this = copy;
}

Server::~Server( void )
{
}

Server& Server::operator=(const Server& target)
{
	if (this != &target)
	{
		(void)target;
	}
	return (*this);
}
/*
Server&	Server::pushToMap(const std::string& key, const std::string& value)
{
	values.insert(std::pair<std::string, std::string>(key, value));
	return (*this);
}

Server&	Server::pushToVector(const Location& value)
{
	locations.push_back(value);
	return (*this);
}

void	Server::displayMapContent( void )
{
	std::map<std::string, std::string>::iterator iterBegin = values.begin();
	std::map<std::string, std::string>::iterator iterEnd = values.end();
	while (iterBegin != iterEnd)
	{
		std::cout << "Key ==> " << iterBegin->first << "   |   Value ==> " << iterBegin->second << std::endl;
		iterBegin++;
	}
}

void	Server::displayVectorContent( void )
{

}*/
