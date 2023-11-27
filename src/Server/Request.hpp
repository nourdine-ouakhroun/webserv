
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "RequestLine.hpp"


class Request
{
private:
	std::string					request;

	// std::vector<std::string>	requestLine;
	maps						headerRequest;
	maps						bodyRequest;
	maps						query;
	maps						mimeType;
public:
	Request( void );
	~Request( void );
	Request( std::string );
	void setMimeType();

	int			errorExit(int status, std::string errorMsg);
	// std::string	readRequest();
	
	RequestLine		parse(std::string);
	void		parseLine(std::string);
	void		parseHeader(std::string h1);
	
	
	void		displayReq( int HorB );



	// void 	parseBody(std::string h1, int hOrb);

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
