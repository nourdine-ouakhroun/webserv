#ifndef STATUS_HPP
#define STATUS_HPP

#include "String.hpp"

class ResponseHeader
{
    String  _protocol;
    String  _status;
    String  _server;
    String  _contentType;
    String  _contentLength;
    String  _location;
    String  _connection;
    String  _body;
    public:
        ResponseHeader( void );
        ResponseHeader& protocol(const String& _protocol);
        ResponseHeader& status(const String& _status);
        ResponseHeader& server(const String& server);
        ResponseHeader& contentType(const String& _contentType);
        ResponseHeader& contentLength(const String& _contentLength);
        ResponseHeader& location(const String& _location);
        ResponseHeader& connection(const String& _connection);
        ResponseHeader& body(const String& _body);
        String  toString( void );

};

#endif
