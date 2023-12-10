#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "StatusCode.hpp"
#include "Request.hpp"

class Request;
class Response
{
private:
    std::string         response;
    maps                mimeType;
    const std::string   ENDLINE;
public:
    Response( void );
    ~Response( void );

    void    addRequestLine( const std::string &version, const std::string &statusCode, const std::string &msg);
    void    addHeader( std::string key, std::string value);
    void    addBlankLine( void );
    void    addBody( std::string content);

    void    setMimeType( void );
    
    std::string getMimeType( const std::string &key) const;
    const   std::string &getResponse() const;
    void    checkStatus( const StatusCode &statusCode );


};

#endif
