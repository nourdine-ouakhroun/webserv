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
	std::vector<String>	fileContent;
	std::vector<std::vector<String> >	serversContents;
	std::vector<ServerPattern> servers;

	Parser( void );

	public :
		Parser(const String& _fileName);
		Parser(const Parser& copy);
		~Parser( void ) throw();
		Parser& operator=(const Parser& target);
		const std::vector<std::vector<String> >& getServersContents( void ) const;
		const	std::vector<ServerPattern>&	getServers( void ) const;
		static std::vector<Data>	parseHeader(const String& header);
		
		void	printServerModel(const ServerPattern& server);
	
	private :
		Data	extractDataFromString(String& line);
		void	 printLocations(const LocationPattern& locs);
		LocationPattern	 getLocations(std::vector<String>::iterator& begin, const std::vector<String>::iterator& end, String	path);
		void	 getFileContent( void );
		std::vector<String>	 getServerConfig(std::vector<String>::iterator&, const std::vector<String>::iterator&);
		void	parsingFile(std::vector<String> content);
		void	splitContentIntoServers( void );
		void	getFinalResualt( void );
		void	checkSyntax( void );
		String	readFile( void );
		void	checkServerKeys( void );
		void	checkLocationKeys(const std::vector<LocationPattern>& loca, const std::vector<String>& keys);
};

#endif
