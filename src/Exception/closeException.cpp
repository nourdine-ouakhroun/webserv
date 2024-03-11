#include "closeException.hpp"

closeException::closeException( void ) : exception(), message()
{
}

closeException::~closeException( void ) throw()
{
}


const char*	closeException::what( void ) const throw()
{
	return (message.c_str());
}
