#ifndef PARSSINGREQUIST_HPP
#define PARSSINGREQUIST_HPP
#include"Server.hpp"
#include "webserver.h"
#include <map>
#include <vector>

struct _requset{
	vector<String> requistLine;
	map<String, vector<String> > header;
	vector<String> body;
};

class ParssingRequist
{
	private :

		ParssingRequist( void );
	public :
		static _requset setreq( vector<String> );
		~ParssingRequist();
		static Data	SpliteEvryLine( String );
		static vector<String>	SplitBynewLine( String );
};
#endif
