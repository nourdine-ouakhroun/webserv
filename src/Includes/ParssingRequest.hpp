#ifndef PARSSINGRequest_HPP
#define PARSSINGRequest_HPP
#include"Server.hpp"
#include "Data.hpp"
#include <map>
#include <vector>

struct _requset{
	vector<String> RequestLine;
	map<String, vector<String> > header;
	vector<String> body;
};

class ParssingRequest
{
	private :

		ParssingRequest( void );
	public :
		static _requset setreq( vector<String> );
		~ParssingRequest();
		static Data	SpliteEvryLine( String );
		static vector<String>	SplitBynewLine( String );
};
#endif
