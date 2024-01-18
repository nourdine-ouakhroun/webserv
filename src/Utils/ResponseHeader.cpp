#include "ResponseHeader.hpp"

ResponseHeader::ResponseHeader( void )
{
    this->_protocol    = "HTTP/1.1";
    this->_status      = "200 OK";
    this->_server      = "webserv";
    _body = NULL;
}

ResponseHeader::~ResponseHeader( void )
{
    delete _body;
    _body = NULL;
}


ResponseHeader::ResponseHeader(const ResponseHeader& copy)
{
    _body = NULL;
    *this = copy;
}

ResponseHeader& ResponseHeader::operator=(const ResponseHeader& target)
{
    if (this != &target)
    {
        _protocol = target._protocol;
        _status = target._status;
        _server = target._server;
        _contentType = target._contentType;
        _contentLength = target._contentLength;
        _location = target._location;
        _connection = target._connection;
        _fileName = target._fileName;
        if (target._body)
        {
            delete _body;
            _body = new String(*target._body);
        }
    }
    return (*this);
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

ResponseHeader& ResponseHeader::body(String* _body)
{
    this->_body = _body;
    return (*this);
}

ResponseHeader& ResponseHeader::fileName(const String& fileName)
{
    this->_fileName = fileName;
    return (*this);
}

String ResponseHeader::getFileName( void ) const
{
    return (this->_fileName);
}


String*  ResponseHeader::toString( void )
{
    String* response = new String(this->_protocol + " " + this->_status + "\r\n");
    response->append("Server: " + this->_server + "\r\n");
    if (!this->_contentLength.empty())
        response->append("Content-Type: " + this->_contentType + "\r\n");
    if (!this->_contentLength.empty())
        response->append("Content-Length: " + this->_contentLength + "\r\n");
    if (!this->_connection.empty())
        response->append("Connection: " + this->_connection + "\r\n");
    if (!this->_location.empty())
        response->append("Location: " + this->_location + "\r\n");
    response->append("\r\n");
    if (this->_body)
        response->append(*(this->_body));
    return (response);
}