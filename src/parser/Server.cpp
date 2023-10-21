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
