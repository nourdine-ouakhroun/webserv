#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sys/stat.h>
#include <unistd.h>

#include "ServerData.hpp"
#include "String.hpp"

typedef std::map<std::string, std::string> maps;
std::vector<std::string> split(const std::string &line, const std::string &sep);

class Response;
class Request
{
    private:
        
        string  method;
        string  path;
        string  query;
        string  version;
        string  boundary;
        string  contentType;
        maps    _header;

        // vector< vector<pair<string, string> > > formData;
        
        string	Header;
	    string	Body;
        
        vector<pair<string, string> > upload;

        
        // maps    _body;
    public:
        Request( void );
        ~Request( void );

        // ------------------------------ parsing request ----------------------------------
        void                        parseRequest(const std::string &request);
        void                        parseRequestLine( const string& reqLine );
        void                        parseHeader();
        void                        parseUrl( string url );

        void                        unchunked();
        void                        parseBody();
        void                        parseMultipartFormData(const std::string& payload, const std::string& boundary);
        // ---------------------------------------------------------------------------------

        // --------------------------------------- geters -----------------------------------
        const std::string   &getMethod( void ) const;
        const std::string   &getPath( void ) const;
        const std::string   &getQuery( void ) const;
        const std::string   &getVersion( void ) const;
        const std::string   &getBoundary() const;
        const std::string   &getContentType() const;


        const maps          &getHeader( void ) const;
        const std::string   &getBody( void ) const;
        const vector<pair<string, string> > getUploads() const;

        const std::string   &header(const std::string &key) const;
        std::string         extention(const string& path) const;
        // -----------------------------------------------------------------------------------
};
#endif
