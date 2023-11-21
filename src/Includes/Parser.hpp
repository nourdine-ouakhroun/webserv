#ifndef	PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include "Location.hpp"
#include <cctype>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include "ParsingException.hpp"
#include "ServerModel.hpp"

#ifndef KEYS
	#define KEYS "autoindex error_log index access_log error_page aio aio_write alias auth_delay chunked_transfer_encoding client_body_buffer_size client_body_in_file_only client_body_in_single_buffer client_body_temp_path client_body_timeout client_header_buffer_size client_header_timeout client_max_body_size connection_pool_size default_type directio directio_alignment disable_symlinks error_page etag http if_modified_since ignore_invalid_headers internal keepalive_disable keepalive_requests keepalive_time keepalive_timeout large_client_header_buffers limit_except limit_rate limit_rate_after lingering_close lingering_time lingering_timeout listen location log_not_found log_subrequest max_ranges merge_slashes msie_padding msie_refresh open_file_cache open_file_cache_errors open_file_cache_min_uses open_file_cache_valid output_buffers port_in_redirect postpone_output read_ahead recursive_error_pages request_pool_size reset_timedout_connection resolver resolver_timeout root satisfy send_lowat send_timeout sendfile sendfile_max_chunk server server_name server_name_in_redirect server_names_hash_bucket_size server_names_hash_max_size server_tokens subrequest_output_buffer_size tcp_nodelay tcp_nopush try_files types types_hash_bucket_size types_hash_max_size underscores_in_headers variables_hash_bucket_size variables_hash_max_size"
#endif


class Parser
{
	String	fileName;
	std::vector<String>	fileContent;
	std::vector<std::vector<String> >	serversContents;
	std::vector<ServerModel> servers;

	Parser( void );

	public :
		Parser(const String& _fileName);
		Parser(const Parser& copy);
		~Parser( void ) throw();
		Parser& operator=(const Parser& target);
		const std::vector<std::vector<String> >& getServersContents( void ) const;
		const	std::vector<ServerModel>&	getServers( void ) const;
		static std::vector<Data>	parseHeader(const String& header);
		
		void	printServerModel(const ServerModel& server);
	
	private :
		Data	extractDataFromString(String& line);
		void	 printLocations(const Location& locs);
		Location	 getLocations(std::vector<String>::iterator& begin, const std::vector<String>::iterator& end, String	path);
		void	 getFileContent( void );
		std::vector<String>	 getServerConfig(std::vector<String>::iterator&, const std::vector<String>::iterator&);
		void	parsingFile(std::vector<String> content);
		void	splitContentIntoServers( void );
		void	getFinalResualt( void );
		void	checkSyntax( void );
		String	readFile( void );
		void	checkServerKeys( void );
		void	checkLocationKeys(const std::vector<Location>& loca, const std::vector<String>& keys);
};

#endif
