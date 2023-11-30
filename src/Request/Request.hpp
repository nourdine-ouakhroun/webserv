
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <sstream>
#include <fstream>
#include <ctime>

#include "RequestLine.hpp"
#include "RequestHeader.hpp"
#include "RequestBody.hpp"


class Request
{
private:
	std::string					request;

	RequestLine					requestLine;
	RequestHeader				requestHeader;
	RequestBody					requestBody;


	// std::vector<std::string>	requestLine;

	
	maps						mimeType;


public:
	Request( void );
	~Request( void );
	Request( std::string );

    // set attribute
	// void			setMimeType();

    // get attribute
	RequestLine		getRequestLine() const;
	RequestHeader	getRequestHeader() const;
	RequestBody		getRequestBody() const;

	int			errorExit(int status, std::string errorMsg);
	// std::string	readRequest();
	
	void		parseRequest(std::string);
	// void		parseLine(std::string);
	void		parseHeader(std::string h1);
	
	
	// void		displayReq( int HorB );



	void 	parseBody(std::string h1, int hOrb);

	// start from here
	// void 	requestIsFormed();
	// void 	statusCodeError(int statusCode, std::string phrase);
	// const std::vector<std::string> &getRequestLine() const
	// {
	// 	return(requestLine);
	// }
	// std::string methodeIs( void )
	// {
	// 	return (requestLine[0]);
	// }
	void setMimeType();
	std::string getMimeType(const std::string &extension)
	{
		if (mimeType[extension].empty())
		{
			return ("Content-Type: " + mimeType[".html"]);
		}
		return ("Content-Type: " + mimeType[extension]);
	}
};


std::string	readRequest(int clientFd);


#endif
