#include "Request.hpp"

std::vector<std::string> split(std::string line, std::string sep)
{
	std::vector<std::string> sp;
	size_t pos = 0;
	while ((pos = line.find(sep)) != std::string::npos)
	{
		sp.push_back(line.substr(0, pos));
		line = line.substr(pos + sep.length(), line.length());
	}
	sp.push_back(line.substr(0, pos));
	return (sp);
}

void Request::displayReq( void )
{

	std::cout << "---------------------- [ParseReq] ---------------------" << std::endl;
	for(size_t i = 0; i < requestLine.size(); i++)
		std::cout << requestLine[i] << std::endl;

	for(smap::iterator it = headerRequest.begin(); it != headerRequest.end(); it++)
		std::cout << it->first << "|" << it->second << std::endl;

	// for(smap::iterator it = bodyRequest.begin(); it != bodyRequest.end(); it++)
	// 	std::cout << it->first << "|" << it->second << std::endl;
}

int	Request::errorExit(int status, std::string errorMsg)
{
	if (status < 0)
	{
		std::cerr << errorMsg << std::endl;
		exit (1);
	}
	return (status);
}

std::string	Request::readRequest(const int &clientFd)
{
	std::string	request;
	int			byte = 0;
	int			readbyte = 1;
	char		buf[readbyte + 1];
	while(1)
	{
		byte = read(clientFd, buf, readbyte);
		// std::cout << byte << " " << readbyte << std::endl;
		if (byte <= 0)
			break ;
		buf[byte] = 0;
		request += buf;
		if (byte < readbyte)
			break ;

		// if(request.find("\r\n\r\n") != std::string::npos)
		// 	break;
	}
	return (request);
}


void Request::parseLine(std::string line)
{
	size_t		pos = 0;
	std::string key;
	std::string value;
	if ((pos = line.find(": ")) != std::string::npos)
	{
		key   = line.substr(0, pos);
		value = line.substr(pos + 2, line.length());
	}
	headerRequest[key] = value;
}

void Request::parseHeader(std::string h1, int hOrb)
{
	size_t	newlinePos = 0;
	size_t	pos = 0;
	std::string line;

	while ((newlinePos = h1.find("\r\n")) != std::string::npos)
	{
		line = h1.substr(0, newlinePos);

		if ((pos = line.find(": ")) != std::string::npos)
		{
			std::string key   = line.substr(0, pos);
			std::string value = line.substr(pos + 2, line.length());
			if (hOrb == 1)
				headerRequest[key] = value;
			// else if (hOrb == 2)
			// 	bodyRequest[key] = value;
		}
		h1 = h1.substr(newlinePos + 2, h1.length());
	}
}

// void Request::parseBody(std::string h1, int hOrb)
// {
// 	size_t	newlinePos = 0;
// 	size_t	pos = 0;
// 	std::string line;

// 	while ((newlinePos = h1.find("\n")) != std::string::npos)
// 	{
// 		line = h1.substr(0, newlinePos);

// 		if ((pos = line.find(": ")) != std::string::npos)
// 		{
// 			std::string key   = line.substr(0, pos);
// 			std::string value = line.substr(pos + 2, line.length());
// 			if (hOrb == 1)
// 				headerRequest[key] = value;
// 			// else if (hOrb == 2)
// 			// 	bodyRequest[key] = value;
// 		}
// 		h1 = h1.substr(newlinePos + 1, h1.length());
// 	}
// }
void    Request::checkRequest(std::string request)
{
	std::string		line;
	std::string 	headerReq;
	std::string		bodyReq;
	// size_t						pos = 0;
	// int							check = 0;
	
	// std::cout << "----------------------   [Request]   ---------------------" << std::endl;
	// std::cout << request << std::endl;
	// std::cout << "++++++++++++++++++++++ [End Request] +++++++++++++++++++++" << std::endl;

	size_t headerPos = request.find("\r\n\r\n");
	if (headerPos != std::string::npos)
	{
		size_t start = request.find("\r\n");
		if (start != std::string::npos)
		{
			std::string line = request.substr(0, start);
			requestLine = split(line, " ");
		}
		headerReq = request.substr(start + 2, headerPos);
		bodyReq = request.substr(headerPos + 4, request.length());
	}

	// std::cout << bodyReq << std::endl;
	parseHeader(headerReq, 1);
	// parseBody(bodyReq, 2);

	// bodyRequest.clear();

}

Request::Request( void )
{
}

Request::~Request( void )
{
}
