#include "ParsingException.hpp"

ParsingException::ParsingException( void ) : exception(), message()
{
}

ParsingException::ParsingException(const String& Message) : exception(), message(Message)
{
}

ParsingException::ParsingException(const ParsingException& copy) : exception(copy)
{
	*this = copy;
}
ParsingException::~ParsingException( void ) throw()
{
}

ParsingException&	ParsingException::operator=(const ParsingException& target)
{
	if (this != &target)
	{
		exception::operator=(target);
		message = target.message;
	}
	return (*this);
}

const char*	ParsingException::what( void ) const throw()
{
	return (message.c_str());
}
