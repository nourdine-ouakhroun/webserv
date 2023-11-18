
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include <vector>

typedef std::map<std::string, std::string> smap;

class Request
{
private:
	std::vector<std::string>	requestLine;
	smap						headerRequest;
	smap						bodyRequest;
public:
	Request( void );
	~Request( void );

	int			errorExit(int status, std::string errorMsg);
	std::string	readRequest(const int &clientFd);
	void		checkRequest(std::string);
	void		parseLine(std::string);
	void		displayReq();



	void	parseHeader(std::string h1, int hOrb);
	// void 	parseBody(std::string h1, int hOrb);
};




#endif
