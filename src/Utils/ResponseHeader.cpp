#include "ResponseHeader.hpp"

ResponseHeader::ResponseHeader( void )
{
    this->_protocol    = "HTTP/1.1";
    this->_status      = "200 OK";
    this->_server      = "webserv";
}

ResponseHeader& ResponseHeader::protocol(const String& _protocol)
{
    this->_protocol = _protocol;
    return (*this);
}

ResponseHeader& ResponseHeader::status(const String& _status)
{
    this->_status = _status;
    return (*this);
}

ResponseHeader& ResponseHeader::server(const String& _server)
{
    this->_server = _server;
    return (*this);
}


ResponseHeader& ResponseHeader::contentType(const String& _contentType)
{
    this->_contentType = _contentType;
    return (*this);
}

ResponseHeader& ResponseHeader::contentLength(const String& _contentLength)
{
    this->_contentLength = _contentLength;
    return (*this);
}

ResponseHeader& ResponseHeader::location(const String& _location)
{
    this->_location = _location;
    return (*this);
}

ResponseHeader& ResponseHeader::connection(const String& _connection)
{
    this->_connection = _connection;
    return (*this);
}

ResponseHeader& ResponseHeader::body(const String& _body)
{
    this->_body = _body;
    return (*this);
}

String  ResponseHeader::toString( void )
{
    String response(this->_protocol + " " + this->_status + "\r\n");
    response.append("Server: " + this->_server + "\r\n");
    if (!this->_contentLength.empty())
        response.append("Content-Type: " + this->_contentType + "\r\n");
    if (!this->_contentLength.empty())
        response.append("Content-Length: " + this->_contentLength + "\r\n");
    if (!this->_connection.empty())
        response.append("Connection: " + this->_connection + "\r\n");
    if (!this->_location.empty())
        response.append("Location: " + this->_location + "\r\n");
    response.append("\r\n");
    response.append(this->_body);
    return (response);
}