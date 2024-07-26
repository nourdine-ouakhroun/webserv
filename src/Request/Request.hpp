#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sys/stat.h>
#include <unistd.h>

#include "ServerData.hpp"
#include "String.hpp"





class Response;
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
        void                                parseRequest(const string &request);
        void                                parseRequestLine( const string& reqLine );
        void                                parseHeader();
        void                                parseUrl( const string& url );

        // void                                unchunked();
        void                                parseBody();
        void                                parseMultipartFormData(const string& payload, const string& boundary);
        // ---------------------------------------------------------------------------------

        // --------------------------------------- geters -----------------------------------
        const string                   &getMethod( void ) const;
        const string                   &getPath( void ) const;
        const string                   &getQuery( void ) const;
        const string                   &getVersion( void ) const;
        const string                   &getBoundary() const;
        const string                   &getContentType() const;
        const map<string, string>           &getHeaders( void ) const;
        const string                        &getHeader( void ) const;
        const string                        &getBody( void ) const;
        const vector<pair<string, string> > getUploads() const;
        string                         header(const string &key) const;
        string                         extention(const string& path) const;
        // -----------------------------------------------------------------------------------
};
vector<string> split(const string &line, const string &sep);

#endif
