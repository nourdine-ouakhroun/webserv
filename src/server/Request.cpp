#include "Request.hpp"

int	Request::errorExit(int status, std::string errorMsg)
{
	if (status < 0)
	{
		std::cerr << errorMsg << std::endl;
		exit (1);
	}
	return (status);
}

void Request::readRequest(int &clientFd)
{
	// std::string	data;
	int			byte = 0;
	int			readbyte = 1;
	char		buf[readbyte + 1];
	while(1)
	{
		byte = errorExit(read(clientFd, buf, readbyte), "Error : read Faile!.\n");
		buf[byte] = 0;
		request += buf;
		if(request.find("\r\n\r\n") != std::string::npos)
			break;
	}

	checkRequest();
}

void Request::displayReq( void )
{

	std::cout << "---------------------- [ParseReq] ---------------------" << std::endl;
	std::cout << methode << std::endl;
	smap::iterator it = reqdata.begin();
	for(; it != reqdata.end(); it++)
	{
		std::cout << it->second << std::endl;
	}
}

void Request::parseLine(std::string line)
{
	size_t	pos = 0;
	std::string key;
	std::string value;
	if ((pos = line.find(": ")) != std::string::npos)
	{
		key = line.substr(0, pos);
		value= line.substr(pos + 2, line.length());
	}
	
	reqdata[key] = value;
	/*
	std::cout << "line     : " << line << std::endl;
	std::cout << "key   is : " << key << std::endl;
	std::cout << "value is : " << value << std::endl;
	*/
	//std::cout << "value is : " << reqdata[key] << std::endl;
}

void    Request::checkRequest()
{
	std::string					line;
	size_t						pos = 0;
	int							i = 0;
	
	std::cout << "---------------------- [Request] ---------------------" << std::endl;
	std::cout << request << std::endl;
	while ((pos = this->request.find("\r\n")) != std::string::npos)
	{
		if (request == "\r\n")
			break ;
		line = this->request.substr(0, pos);
		if (i == 0)
			methode = line;
		else
			this->parseLine(line);
		request = this->request.substr(pos + 2, this->request.length());
		i++;
	}
	displayReq();
	reqdata.clear();
	request.clear();

}

Request::Request( void )
{
}

Request::~Request( void )
{
}
