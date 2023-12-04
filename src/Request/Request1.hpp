
#ifndef REQUEST_HPP
#define REQUEST_HPP





#include "GetMethod.hpp"

class Request
{
private:
	// std::string	request;

	std::string	requestLine;
	std::string	requestHeader;
	std::string	requestBody;

	// PostMethod post;
	// DeleteMethod Delete;


	
public:
	Request( void );
	~Request( void );


	int			errorExit(int status, std::string errorMsg);
	
	GetMethod		parseRequest( std::string request );
};





#endif
