#ifndef PARSSINGREQUIST_HPP
#define PARSSINGREQUIST_HPP
#include"Server.hpp"
#include "Data.hpp"
#include <map>
#include <vector>

struct _requset{
	String requistLine;
	std::map<String, std::vector<String> > header;
	std::vector<String> body;
};

class ParssingRequist
{
	private :

		ParssingRequist( void );
	public :
		static _requset setreq( std::vector<String> );
		~ParssingRequist();
		static Data	SpliteEvryLine( String );
		static std::vector<String>	SplitBynewLine( String );
};
#endif
