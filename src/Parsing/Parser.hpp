#ifndef	PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <cctype>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include "ParsingException.hpp"
#include "Patterns/ServerPattern.hpp"

#ifndef KEYS
	#define KEYS "return autoindex error_log index access_log error_page alias client_body_buffer_size client_max_body_size error_page listen location root server server_name try_files types "
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
		Parser& operator=(const Parser& target);
		const vector<vector<String> >& getServersContents( void ) const;
		const	vector<ServerPattern>&	getServers( void ) const;
		static vector<Data>	parseHeader(const String& header);
		
		void	printServerPattern(const ServerPattern& server);
	
	private :
		Data	extractDataFromString(String& line);
		void	 printLocations(const LocationPattern& locs);
		LocationPattern	 getLocations(vector<String>::iterator& begin, const vector<String>::iterator& end, String	path);
		void	 getFileContent( void );
		vector<String>	 getServerConfig(vector<String>::iterator&, const vector<String>::iterator&);
		void	parsingFile(vector<String> content);
		void	splitContentIntoServers( void );
		void	getFinalResualt( void );
		void	checkSyntax( void );
		String	readFile( void );
		void	checkServerKeys( void );
		void	checkLocationKeys(const vector<LocationPattern>& loca, const vector<String>& keys);
		void	checkingInfos( void );
};

#endif
