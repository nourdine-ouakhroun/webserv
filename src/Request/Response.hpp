#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include <unistd.h>

#include "Request.hpp"
#include "ServerModel.hpp"

typedef std::map<std::string, std::string> maps;
#define VERSION "HTTP/1.1 "
#define ENDLINE "\r\n"

class Request;
class Response
{
private:
    std::string response;
	int         statusCode;
    std::string msg;
    
    maps        _header;
    std::string _body;

public:
    Response( void );
    ~Response( void );

    void setStatusCode( const int &statusCode );
    void setMsg( const std::string &msg );

    void setHeader(const std::string &key, const std::string &value);
    void setBody(const std::string &body);

    int		getStatusCode( void ) const;
    const	std::string &getVersion( void ) const;
    const	std::string &getMsg( void ) const ;
    const   std::string &getResponse( void ) const;

    //const	std::string &getHeader(const std::string &key, const std::string &value) const;
    //const	std::string &getBody(const std::string &key, const std::string &value) const;
	
	// const std::string &getBody( void ) const;

	void	makeHeaderResponse( void );
	void	makeBodyResponse( void );

    // bool    isAllowed( const std::string &url );
    // bool    isFormed(Request req);
    // bool    isMatched( const Request &req, std::vector<ServerModel> serv);


};

#endif
