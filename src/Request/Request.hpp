
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <map>


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

        void parseRequestLine( std::string reqLine );
        void parseHeader( std::string reqHeader );
        void parseBody( std::string reqBody );

        const std::string& header(const std::string &key);
        void parseUrl();

        const std::string &getMethod( void ) const;
        const std::string &getUrl( void ) const;
        const std::string &getVersion( void ) const;
        const std::string &getPathname( void ) const;
        const std::string &getQuery( void ) const;




	    void parseRequest( std::string request );

};
std::string	readRequest(int clientFd);
std::vector<std::string> split(std::string line, std::string sep);


#endif