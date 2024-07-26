#ifndef	PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <cctype>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include "ParsingException.hpp"
#include "ServerPattern.hpp"


#ifndef KEYS
	#define KEYS "cgi_time method upload_dir cgi include return autoindex error_log index access_log error_page alias client_max_body_size error_page listen location root server server_name servercheck"
#endif

class Parser
{
	String	fileName;
	vector<String>	fileContent;
	vector<vector<String> >	serversContents;
	vector<ServerPattern> servers;

	Parser( void );

	public :
		Parser(const String& _fileName);
		Parser(const Parser& copy);
		~Parser( void ) throw();
		Parser&								operator=(const Parser& target);
		const	vector<vector<String> >&	getServersContents( void ) const;
		const	vector<ServerPattern>&		getServers( void ) const;
		void								printServerPattern(const ServerPattern& server);
	
	private :
		map<string, string>	getMimeTypes(String fileName);
		LocationPattern		getLocations(vector<String>::iterator& begin, const vector<String>::iterator& end, String	path);
		vector<String>		getServerConfig(vector<String>::iterator&, const vector<String>::iterator&);
		String				readFile( void );
		Data				extractDataFromString(String& line);
		void				printLocations(const LocationPattern& locs);
		void	 			getFileContent( void );
		void				parsingFile(vector<String> content);
		void				splitContentIntoServers( void );
		void				getFinalResualt( void );
		void				checkSyntax( void );
		void				checkServerKeys( void );
		void				checkLocationKeys(const vector<LocationPattern>& loca, const vector<String>& keys);
		void				checkingInfos( void );
		void				includeMimeTypes( void );


};

#endif
