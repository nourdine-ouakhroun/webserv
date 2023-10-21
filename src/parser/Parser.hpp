#ifndef	PARSER_HPP
#define PARSER_HPP

#include <iostream>

class Parser
{
	//std::string configFile;

	public :
		Parser( void );
		Parser(const Parser& copy);
		~Parser( void );
		Parser& operator=(const Parser& target);
};

#endif
