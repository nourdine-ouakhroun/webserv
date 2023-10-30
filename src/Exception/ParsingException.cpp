#include "ParsingException.hpp"

ParsingException::ParsingException( void ) : message()
{
}

ParsingException::ParsingException(const String& Message) : message(Message)
{
}

ParsingException::ParsingException(const ParsingException& copy) : std::exception(copy)
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
		std::exception::operator=(target);
		message = target.message;
	}
	return (*this);
}

const char*	ParsingException::what( void ) const throw()
{
	return (message.c_str());
}
