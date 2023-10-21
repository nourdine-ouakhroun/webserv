#include "Parser.hpp"

Parser::Parser( void )
{

}

Parser::Parser(const Parser& copy)
{
	*this = copy;
}

Parser::~Parser( void )
{
}

Parser& Parser::operator=(const Parser& target)
{
	if (this != &target)
	{
		(void) target;
	}
	return (*this);
}
