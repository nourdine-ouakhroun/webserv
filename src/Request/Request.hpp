#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sys/stat.h>
#include "Response.hpp"
#include "ServerData.hpp"

class Response;

typedef std::map<std::string, std::string> maps;

class Request
{
    private:
        std::string method;
        std::string url;
        std::string version;
        std::string pathname;
        std::string query;
        
        maps        _header;
        maps        _body;
    public:
        Request( void );
        ~Request( void );

	    void                        parseRequest( std::string request );
        void                        parseRequestLine( std::string reqLine );
        void                        parseUrl( void );
        void                        parseHeader( std::string reqHeader );
        void                        parseBody( std::string reqBody );

        std::string header(const std::string &key) const;
        std::string body(const std::string &key) const;

        std::string extention( const std::string &path) const;


        const std::string &getMethode( void ) const;
        const std::string &getUrl( void ) const;
        const std::string &getVersion( void ) const;
        const std::string &getPathname( void ) const;
        const std::string &getQuery( void ) const;
        
        const maps &getHeader( void ) const;
        const maps &getBody( void ) const;

        // checkserver

        std::string checkServer(const ServerPattern &server);

        void isFormed(ServerPattern srv, Response &res);
        void isMatched(Response &res);
        void isRedirected(Response &res);
        void isAllowed(Response &res);
        void whichMethode(Response &res);

        // Server check
        std::string getRootFromLocation(ServerPattern server);
        int isDirectory(const std::string& path);
};
// std::std::string	readRequest(FileDepandenc &file);
std::vector<std::string> split(std::string line, std::string sep);

#endif
