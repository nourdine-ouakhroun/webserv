#ifndef PARSSINGREQUIST_HPP
#define PARSSINGREQUIST_HPP
#include"Server.hpp"
#include "Data.hpp"

class ParssingRequist
{
	private :
		std::string RequistContent;
		ParssingRequist( void );
	public :

		ParssingRequist( std::string );
		~ParssingRequist();
		Data	SpliteEvryLine( std::string );
		// std::string			getline();
		std::vector<std::string>	SplitBynewLine(  );
};
#endif
