#include "StatusCode.hpp"

StatusCode::StatusCode( void ) : statusCode(200), version("HTTP/1.1"), msg("OK")
{
}

StatusCode::~StatusCode( void )
{
}

void StatusCode::setStatusCode( const int &statusCode )
{
    this->statusCode = statusCode;
}
void StatusCode::setVersion( const std::string &version )
{
    this->version = version;
}
void StatusCode::setMsg( const std::string &msg )
{
    this->msg = msg;
}
const int &StatusCode::getStatusCode( void ) const
{
    return (this->statusCode);
}
const std::string &StatusCode::getVersion( void ) const
{
    return(this->version);
}
const std::string &StatusCode::getMsg( void ) const
{
    return (this->msg);
}






// bool	allowdSpecialchar(char chr)
// {
// 	std::string specialChar = "_:;.,\\/\"'?!(){}[]@<>=-+*#$&`|~^%";
// 	for(size_t i = 0; i < specialChar.length(); i++)
// 	{
// 		if (chr == specialChar[i])
// 			return (1);
// 	}
// 	return (0);
// }


bool     StatusCode::isAllowed(const std::string &url)
{
	// std::cout << "|" << url << "|" << std::endl;
	for(size_t i = 0; i < url.length(); i++)
	{
		// std::cout << (!isprint(url[i])) << std::endl;
		if (!isprint(url[i]))
			return (0);
	}
	return (1);
}

bool StatusCode::isFormed(Request req)
{
    if ((!req.header("Transfer-Encoding").empty() && req.header("Transfer-Encoding") != "chunked") || !this->isAllowed(req.getPathname()))
    {
        this->statusCode = 501;
        this->version = req.getVersion();
        this->msg = "Not Implemented";
        return (0);
    }
    else if (req.getMethod() == "GET" && req.header("Transfer-Encoding").empty() && req.header("Content-Length").empty())
    {
        this->statusCode = 400;
        this->version = req.getVersion();
        this->msg = "Bad Request";
        return (0);
    }
    else if (req.getPathname().length() > 2048)
    {
        this->statusCode = 414;
        this->version = req.getVersion();
        this->msg = "Request-URI To Long";
        return (0);
    }
    else if (!req.header("Content-Length").empty() && std::atoi(req.header("Content-Length").c_str()) > 1024 )
    {
        this->statusCode = 413;
        this->version = req.getVersion();
        this->msg = "Request Entity To Long";
        return (0);
    }
    return (1);
}

bool StatusCode::isMatched( const std::string &path)
{
    std::cout << path << std::endl;
    return (1);
}

