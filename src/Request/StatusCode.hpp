#ifndef STATUSCODE_HPP
#define STATUSCODE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <map>
#include "Request.hpp"

class Request;
class StatusCode
{
private:

    int         statusCode;
    std::string version;
    std::string msg;
public:
    StatusCode( void );
    ~StatusCode( void );

    void setStatusCode( const int &statusCode );
    void setVersion( const std::string &version );
    void setMsg( const std::string &msg );


    const int &getStatusCode( void ) const;
    const std::string &getVersion( void ) const;
    const std::string &getMsg( void ) const ;

    bool    isAllowed( const std::string &url );
    bool    isFormed(Request req);
    bool    isMatched( const std::string &path );


};

#endif