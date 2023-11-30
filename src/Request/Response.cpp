#include "Response.hpp"

Response::Response( void )
{
}

Response::~Response( void )
{
}

void    Response::setResponseLine(std::string version, std::string statusCode, std::string msg)
{
    response += version+" "+statusCode+" "+msg+"\r\n";
}
void    Response::setHeader(std::string key, std::string value)
{
    response += key+": "+value+"\r\n";
}
void    Response::setEndHeader()
{
    response += "\r\n";
}
void    Response::setBody(std::string content)
{
    response += content;
}

std::string Response::getResponse() const
{
    return (this->response);
}
