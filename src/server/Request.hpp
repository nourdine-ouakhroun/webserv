
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <map>

typedef std::map<std::string, std::string> smap;

class Request
{
private:
	std::string	request;
	std::string	methode;
	smap		reqdata;
public:
	Request( void );
	~Request( void );

	int			errorExit(int status, std::string errorMsg);
	void		readRequest(int &clientFd);
	void		checkRequest();
	void		parseLine(std::string);
	void		displayReq();	
};




#endif
