#include "Response.hpp"

Response::Response( void ) : response(""), ENDLINE("\r\n")
{
    this->setMimeType();
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
std::string Response::getMimeType( const std::string &key) const
{
    for (maps::const_iterator it = this->mimeType.begin(); it != this->mimeType.end(); it++)
	{
		if (it->first == key)
			return (it->second);
	}
	return ("");
}

void Response::checkStatus( const StatusCode &statusCode )
{
    this->addRequestLine("HTTP/1.1" , std::to_string(statusCode.getStatusCode()), statusCode.getMsg());
    // this->addRequestLine("HTTP/1.1" , std::to_string(statusCode.getStatusCode()), statusCode.getMsg());
    // this->addHeader("Server", "webServ");
	this->addBlankLine();
    this->addBody(statusCode.getMsg());
}


void Response::setMimeType( void )
{
    this->mimeType[".csv"] = "text/csv";
    this->mimeType[".doc"] = "application/msword";
    this->mimeType[".css"] = "text/css";
    this->mimeType[".gif"] = "image/gif";
    this->mimeType[".html"] = "text/html";
    this->mimeType[".ico"] = "image/vnd.microsoft.icon";
    this->mimeType[".js"] = "text/javascript";
    this->mimeType[".mp3"] = "audio/mpeg";
    this->mimeType[".mp4"] = "video/mp4";
    this->mimeType[".mpeg"] = "video/mpeg";
}
