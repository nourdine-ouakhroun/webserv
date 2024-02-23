#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sys/stat.h>
#include <unistd.h>

#include "ServerData.hpp"
#include "String.hpp"

typedef std::map<std::string, std::string> maps;
std::vector<std::string> split(std::string line, std::string sep);

class Request
{
    private:
        string  method;
        string  path;
        string  query;
        string  version;
        
        string	Header;
	    string	Body;

        maps    _header;
    public:
        Request( void );
        ~Request( void );

        // ------------------------------ parsing request ----------------------------------
        void                        parseRequest(const std::string &request);
        void                        parseRequestLine( const string& reqLine );
        void                        parseHeader();
        void                        parseUrl( string url );

        void                        parseBody();
        // ---------------------------------------------------------------------------------

        // --------------------------------------- geters -----------------------------------
        const std::string   &getMethod( void ) const;
        const std::string   &getPath( void ) const;
        const std::string   &getQuery( void ) const;
        const std::string   &getVersion( void ) const;

        const maps          &getHeader( void ) const;
        const std::string   &getBody( void ) const;

        const std::string   &header(const std::string &key) const;
        std::string         extention(const string& path) const;
        // -----------------------------------------------------------------------------------
};
#endif
