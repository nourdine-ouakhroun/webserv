
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include <vector>
#include <sstream>

typedef std::map<std::string, std::string> smap;

class Request
{
private:
	std::vector<std::string>	requestLine;
	smap						headerRequest;
	int 						clientFd;
	// smap						bodyRequest;
public:
	Request( void );
	~Request( void );
	Request( int clientFd );

	int			errorExit(int status, std::string errorMsg);
	std::string	readRequest();
	
	void		parseReq(std::string);
	void		parseLine(std::string);
	void		parseHeader(std::string h1);
	
	
	void		displayReq();



	// void 	parseBody(std::string h1, int hOrb);

	// start from here
	void 	requestIsFormed();
	void 	statusCodeError(int statusCode, std::string phrase);
};




#endif
