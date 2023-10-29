#ifndef	PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include "../Models/ServerModel.hpp"

class Parser
{
	std::vector<std::vector<String> >	serversContents;
	std::vector<ServerModel> servers;
	//std::string configFile;

	public :
		Parser( void );
		Parser(const Parser& copy);
		~Parser( void );
		Parser& operator=(const Parser& target);
};

#endif
