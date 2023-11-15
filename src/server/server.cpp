#include "Server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <ctime>

#include "Request.hpp"


std::string readHtml(std::string file)
{
	std::ifstream	in;
	std::string		content;
	std::string		input;

	in.open(file);
	if (in.is_open()) {
		while (std::getline(in, input,'\n'))
		{
			content += input;
		}
	}
	return (content);
}

// void checkRequest(std::string request)
// {
// 	std::string					line;
// 	std::vector<std::string>	content;
// 	size_t						pos = 0;

// 	while ((pos = request.find("\r\n")) != std::string::npos)
// 	{
// 		if (request == "\r\n")
// 			break ;
// 		line = request.substr(0, pos);
// 		request = request.substr(pos + 2, request.length());
// 		// line += '\n';
// 		content.push_back(line);

// 	}
// 	// line = line.substr(0, line.length() - 1);
// 	size_t i = 0;
// 	while (i < content.size())
// 	{
// 		std::cout << content[i];
// 		i++;
// 	}
// }

int	errorExit(int status, std::string errorMsg)
{
	if (status < 0)
	{
		std::cerr << errorMsg << std::endl;
		exit (1);
	}
	return (status);
}

Server::Server()
{
	bzero(&servAddr ,sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	servFd = errorExit(socket(AF_INET, SOCK_STREAM, 0), "Error : socket!.");
	errorExit(bind(servFd, (SA *)&servAddr, sizeof(servAddr)), "Error : bind!.");
	errorExit(listen(servFd, 10), "Error : listen!.");

	int countReq = 1 ;
	Request req;
	while (1) {
		std::cout << "http://localhost:" << PORT << std::endl;
		clientFd = errorExit(accept(servFd, (SA *) NULL, NULL), "Error : accept!.");
		req.readRequest(clientFd);
		
		std::cout << "countReq : " << countReq++ << std::endl;
		std::cout << "-----------------------------------------------------" << std::endl;
		std::string	response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"+readHtml("html.html");
		write(clientFd, response.c_str(), strlen(response.c_str()));
		close(clientFd);
	}
}

Server::~Server()
{
	close(servFd);
	// close(newfd);
}
