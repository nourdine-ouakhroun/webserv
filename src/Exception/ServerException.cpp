#include "ServerException.hpp"

ServerException::ServerException( void ) : exception(), message()
{
}

ServerException::ServerException(const String& Message) : exception(), message(Message)
{
}

ServerException::ServerException(const ServerException& copy) : exception(copy)
{
	*this = copy;
}
ServerException::~ServerException( void ) throw()
{
}

ServerException&	ServerException::operator=(const ServerException& target)
{
	if (this != &target)
	{
		exception::operator=(target);
		message = target.message;
	}
	return (*this);
}

const char*	ServerException::what( void ) const throw()
{
	return (message.c_str());
}
