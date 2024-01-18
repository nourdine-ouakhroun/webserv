#ifndef DIRECTIVES_HPP
#define DIRECTIVES_HPP

#include "webserver.h"

class Directives
{
	private:
		GeneralPattern  data;
		String          path;
	public:
		Directives(const GeneralPattern &_data, const String& _path);
		~Directives( void );
		void			handleLogges( void );
		ResponseHeader	errorPage(String errorNumber, String message);
		ResponseHeader	autoIndexing( void );
		ResponseHeader	tryFiles( void );
		String			indexing( void );
		String			getRootPath( void );
		String			getAliasPath( void );
		ResponseHeader	returnDirective( void );
		static map<String, String> getStatusCode( void );
};

#endif