#ifndef PARSSINGREQUIST_HPP
#define PARSSINGREQUIST_HPP
#include"Server.hpp"
#include "Data.hpp"
#include <map>
#include <vector>

class ParssingRequist
{
	private :
		std::map<String, std::vector<String> > requsteContent;
		String requistLine;
		ParssingRequist( void );
	public :
		ParssingRequist( String );
		void setreq( std::vector<String> );
		~ParssingRequist();
		static Data	SpliteEvryLine( String );
		static std::vector<String>	SplitBynewLine( String );
};
#endif
