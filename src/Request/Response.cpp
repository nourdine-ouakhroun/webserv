#include "Response.hpp"

Response::Response( void ) : response(""), ENDLINE("\r\n")
{
}

Response::~Response( void )
{
}

void Response::addRequestLine( const std::string &version, const std::string &statusCode, const std::string &msg)
{
    this->response += version + " " +  statusCode + " " + msg + this->ENDLINE;
}
void Response::addHeader( std::string key, std::string value)
{
    this->response += key + ": " + value + this->ENDLINE; 
}
void Response::addBlankLine( void )
{
    this->response += this->ENDLINE; 
}
void Response::addBody( std::string content )
{
    this->response += content + this->ENDLINE + this->ENDLINE; 
}



const std::string &Response::getResponse() const
{
    return (this->response);
}

void Response::checkStatus( const StatusCode &statusCode )
{
    this->addRequestLine("HTTP/1.1" , std::to_string(statusCode.getStatusCode()), statusCode.getMsg());
    // this->addRequestLine("HTTP/1.1" , std::to_string(statusCode.getStatusCode()), statusCode.getMsg());
    // this->addHeader("Server", "webServ");
	this->addBlankLine();
    this->addBody(statusCode.getMsg());
}


