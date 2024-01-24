#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include <unistd.h>

#include "Request.hpp"
#include "webserver.h"
#include "Servers.hpp"





typedef std::map<std::string, std::string> maps;

#define VERSION "HTTP/1.1 "
#define ENDLINE "\r\n"

class Request;
class Response
{
private:
	int         statusCode;
    std::string msg;

    maps _header;
    map<int, string> errorPage;
    std::string _body;

    std::string response;
    std::string fileToServe;

    const Request *request;

    maps mimeType;

public:
    Response(void);
    ~Response( void );

    void setErrorPage();
    std::string getErrorPage( int status );

        void setResponse(int status);

    void setStatusCode(int statusCode);
    void setMsg(const std::string &msg);
    void setHeader(const std::string &key, const std::string &value);
    void setBody(const std::string &body);
    void setFileToServe(const std::string &fileName);

    // Request     request;
    void setRequest(const Request &request);

    // const	std::string &getVersion( void ) const;
    int		getStatusCode( void ) const;
    const	std::string &getMsg( void ) const ;
    const   std::string &getResponse( void ) const;
    // const Request &getRequest(void) const;

    //const	std::string &getHeader(const std::string &key, const std::string &value) const;
    //const	std::string &getBody(const std::string &key, const std::string &value) const;
	
	// const std::string &getBody( void ) const;

    // void    setRequestLine(int status, std::string msg, std::string version);

    void    makeResponse( void );



    // void checkPathname( const Request &req, const std::string& path );
    // void    send( void );



	void setMimeType(const map<string, string> &mimeTypes);
    std::string getMimeType( const std::string &key ) const;

};

#endif
