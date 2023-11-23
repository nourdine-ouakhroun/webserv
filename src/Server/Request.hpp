
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

typedef std::map<std::string, std::string> maps;

class Request
{
private:
	std::vector<std::string>	requestLine;
	maps						headerRequest;
	maps						bodyRequest;
	// maps						params;
	int 						clientFd;
public:
	Request( void );
	~Request( void );
	Request( int clientFd );

	int			errorExit(int status, std::string errorMsg);
	std::string	readRequest();
	
	void		parseReq(std::string);
	void		parseLine(std::string);
	void		parseHeader(std::string h1);
	
	
	void		displayReq( int HorB );



	// void 	parseBody(std::string h1, int hOrb);

	// start from here
	void 	requestIsFormed();
	void 	statusCodeError(int statusCode, std::string phrase);
	std::string methodeIs( void )
	{
		return (requestLine[0]);
	}
};




#endif
