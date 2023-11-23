#include "Request.hpp"

std::vector<std::string> split(std::string line, std::string sep)
{
	std::vector<std::string> sp;
	if (line.empty())
		return (sp);
	size_t pos = 0;
	while ((pos = line.find(sep)) != std::string::npos)
	{
		sp.push_back(line.substr(0, pos));
		line = line.substr(pos + sep.length(), line.length());
	}
	sp.push_back(line.substr(0, line.length()));
	return (sp);
}

void Request::displayReq( int HorB )
{
	// if	(HorB == 1)
	// {
	// 	std::cout << "---------------------- [ParseReq] ---------------------" << std::endl;
	// 	for(size_t i = 0; i < requestLine.size(); i++)
	// 		std::cout << requestLine[i] << std::endl;
	// }
	if (HorB == 1)
	{
		std::cout << "-------------- [ Header ] ----------------" << std::endl;
		for(maps::iterator it = headerRequest.begin(); it != headerRequest.end(); it++)
			std::cout << it->first << "|" << it->second << std::endl;
	}
	// else if (HorB == 2)
	// {
	// 	std::cout << "-------------- [ Body ] ----------------" << std::endl;
	// 	for(maps::iterator it = headerRequest.begin(); it != headerRequest.end(); it++)
	// 		std::cout << it->first << "|" << it->second << std::endl;
	// }

	// std::cout << "-------------- [ Body ] ----------------" << std::endl;
	// for(maps::iterator it = bodyRequest.begin(); it != bodyRequest.end(); it++)
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

std::string	Request::readRequest()
{
	std::string	request;
	int			byte = 0;
	int			readbyte = 1;
	char		buf[readbyte + 1];
	while(1)
	{
		byte = read(this->clientFd, buf, readbyte);
		if (byte <= 0)
			break ;
		buf[byte] = 0;
		request += buf;
		if (byte < readbyte)
			break ;
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

void Request::parseHeader(std::string h1)
{
	size_t	newlinePos = 0;
	size_t	pos = 0;
	std::string line;

	while ((newlinePos = h1.find("\n")) != std::string::npos)
	{

		line = h1.substr(0, newlinePos);
		// std::cout << "#" << 0 << " " << newlinePos << "#" << std::endl;

		if ((pos = line.find(": ")) != std::string::npos)
		{
			std::string key   = line.substr(0, pos);
			std::string value = line.substr(pos + 2, line.length());
			// std::cout << "key = " << key << " value = " << value << std::endl;
			headerRequest[key] = value;
		}
		h1 = h1.substr(newlinePos + 1, h1.length());
	}
}

void    Request::parseReq(std::string request)
{
	std::string 	headerReq;
	std::string		bodyReq;

	size_t headerPos = request.find("\r\n\r\n");

	if (headerPos != std::string::npos)
	{
		size_t start = request.find("\r\n");
		if (start != std::string::npos)
		{
			size_t i = start + 2;
			std::string line = request.substr(0, start);
			requestLine = split(line, " ");
			while (i < headerPos)
			{
				if (request[i] != '\r')
					headerReq += request[i];
				i++;
			}
			i = headerPos + 4;
			while (i < request.length())
			{
				if (request[i] != '\r')
					bodyReq += request[i];
				i++;
			}
		}
		// if (start + 2 != headerPos)
			// headerReq = request.substr(start + 2, headerPos);
		// bodyReq = request.substr(headerPos + 4, request.length());
	}
	// std::cout << "#" << requestLine[0] << " " << requestLine[1] << " "<< requestLine[2] << "#" << "\n";
	// int i = 0;
	// while (i < headerReq.length())
	// {
	// 	if (headerReq[i] != '\n' && headerReq[i] != '\t' && !isprint(headerReq[i]))
	// 		std::cout << "▐";
	// 	else
	// 		std::cout << headerReq[i];
	// 	i++;
	// }
	// std::cout  << "---------- { after } -----------\n";
	// std::cout << "#" << headerReq << "#" << std::endl;
	// std::cout << "#" << bodyReq << "#" << std::endl ;
	// std::cout  << "--------------------------------\n";
	parseHeader(headerReq);
	// std::cout  << "--------------------------------\n";
	// std::cout << "#" << bodyReq << "#" << std::endl ;
	// std::cout  << "---------- { before } -----------\n";
	// std::vector<std::string> ss = split("hello world 1337", "@");
	// std::cout << "size = " << ss.size() << std::endl;
	// displayReq();

	// size_t i = 0;
	// while (i < contentType.size())
	// {
	// 	std::cout << contentType[i] << std::endl;
	// 	i++;
	// }
	std::cout << "#" << headerReq << "#" << std::endl;
	std::cout << "#" << bodyReq << "#" << std::endl;

	if (methodeIs() == "GET")
	{
		// std::cout << "-------------- [ Header ] ----------------" << std::endl;
		// std::cout << "#" << headerReq << "#" << std::endl;
	}
	else if (methodeIs() == "POST")
	{
		std::cout << "-------------- [ Body ] ----------------" << std::endl;
		std::vector<std::string> contentType = split(headerRequest["Content-Type"], ";");
		std::vector<std::string> boundray;
		if (!contentType.empty())
		{
			if (contentType[0] == "multipart/form-data")
			{
				std::cout << "multipart/form-data" << std::endl;
				boundray = split(contentType[1], "=");
				if (!boundray.empty() && boundray.size() == 2)
				{
					std::cout << boundray[1] << std::endl;
					// std::cout 
				}
				
			}
			else if (contentType[0] == "application/x-www-form-urlencoded")
			{
				std::cout << "application/x-www-form-urlencoded" << std::endl;
				std::vector<std::string> content = split(bodyReq, "&");
				if (!content.empty())
				{
					for (size_t i = 0; i < content.size(); i++)
					{
						std::vector<std::string> keyValue = split(content[i], "=");
						if (!keyValue.empty())
							bodyRequest[keyValue[0]] = keyValue[1];
					}
					for (maps::iterator it = bodyRequest.begin(); it != bodyRequest.end(); it++)
					{
						std::cout << "value = " << it->second << std::endl;
					}
					
				}
			}
		}

		// std::cout << "#" << bodyReq << "#" << std::endl;
	}
	else if (methodeIs() == "DELETE")
	{

	}
	// std::cout << headerRequest["Content-Type"] << std::endl;
	// if (!contentType.empty())
	// {
	// 	std::cout << "POST" << std::endl;
		
	// 	std::cout << contentType[0] << std::endl;
	// 	if (contentType[0] == "multipart/form-data")
	// 	{
	// 		boundray = split(contentType[1], "=");
	// 		if (!boundray.empty())
	// 			std::cout << boundray[1] << std::endl;
	// 	}
	// 	else if (contentType[0] == "application/x-www-form-urlencoded")
	// 	{
	// 		// boundray = split(contentType[1], "=");
	// 		// if (!boundray.empty())
	// 			std::cout << contentType[0] << std::endl;
	// 	}
	// }
	// else
	// 	std::cout << "GET" << std::endl;


	// if (!contentType.empty())
	// 	boundray = split(contentType[1], "=");
	// 	if (!boundray.empty())
	// 		std::cout << boundray[1] << std::endl;


	// std::cout << bodyReq.find()
	// exit(1);
	// std::cout << "-------------- [ Body ] ----------------" << std::endl;
	// std::cout << bodyReq ;
	// displayReq();


	// requestIsFormed();
}


bool	allowdSpecialchar(char chr)
{
	std::string specialChar = "_:;.,\\/\"'?!(){}[]@<>=-+*#$&`|~^%";
	for(size_t i = 0; i < specialChar.length(); i++)
	{
		if (chr == specialChar[i])
			return (1);
	}
	return (0);
}
bool	allowedChar(std::string url)
{
	std::cout << "|" << url << "|" << std::endl;
	for(size_t i = 0; i < url.length(); i++)
	{
		std::cout << (!isprint(url[i])) << std::endl;
		if (!isprint(url[i]))
			return (0);
	}
	return (1);
}
void Request::statusCodeError(int statusCode, std::string phrase)
{
	std::stringstream s;
	s << statusCode;
	std::string code;
	s >> code;

	std::string	response = "HTTP/1.1 " + code + " " + phrase + "\r\n\r\n" + code + " " + phrase;
	// std::cout << code << std::endl;
	// std::cout << response << std::endl;
	write(this->clientFd, response.c_str(), strlen(response.c_str()));
	close(this->clientFd);
}
void Request::requestIsFormed()
{
	std::cout << "from methode requestIsformed()" << std::endl;
	// displayReq();
	if (requestLine.size() == 3 && !allowedChar(requestLine[1]))
		statusCodeError(400, "Bad Request!");
	else if (headerRequest["Transfer-Encoding"].empty() && headerRequest["Content-Length"].empty() && requestLine[0] == "POST")
		statusCodeError(400, "Bad Request!");
	else if (!headerRequest["Transfer-Encoding"].empty() && headerRequest["Transfer-Encoding"] != "chunked")
		statusCodeError(501, "Not Implimented!");
	else if (requestLine[1].length() > 2048)
		statusCodeError(414, "Request URI Too Longe!");
	else if (requestLine[1].length() > 2048)
		statusCodeError(413, "Content Too Large!");
}

Request::Request( void )
{
}
Request::Request( int clientFd )
{
	this->clientFd = clientFd;
}

Request::~Request( void )
{
}
