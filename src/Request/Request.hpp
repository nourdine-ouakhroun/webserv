#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sys/stat.h>
#include <unistd.h>

#include "ServerData.hpp"
#include "String.hpp"


class Request
{
    private:
        
        string                  _method;
        string                  _path;
        string                  _query;
        string                  _version;
        string                  _boundary;
        string                  _contentType;
        map<string, string>     _header;
        
        vector<pair<string, string> > _upload;

        string	                Header;
	    string	                Body;
    public:
        Request( void );
        ~Request( void );

        // ------------------------------ parsing request ----------------------------------
        void                                parseRequest(const std::string &request);
        void                                parseRequestLine( const string& reqLine );
        void                                parseHeader();
        void                                parseUrl( const string& url );

        // void                                unchunked();
        void                                parseBody();
        void                                parseMultipartFormData(const std::string& payload, const std::string& boundary);
        // ---------------------------------------------------------------------------------

        // --------------------------------------- geters -----------------------------------
        const std::string                   &getMethod( void ) const;
        const std::string                   &getPath( void ) const;
        const std::string                   &getQuery( void ) const;
        const std::string                   &getVersion( void ) const;
        const std::string                   &getBoundary() const;
        const std::string                   &getContentType() const;
        const map<string, string>           &getHeaders( void ) const;
        const string                        &getHeader( void ) const;
        const string                        &getBody( void ) const;
        const vector<pair<string, string> > getUploads() const;
        const std::string                   &header(const std::string &key) const;
        std::string                         extention(const string& path) const;
        // -----------------------------------------------------------------------------------
};
std::vector<std::string> split(const std::string &line, const std::string &sep);

#endif
