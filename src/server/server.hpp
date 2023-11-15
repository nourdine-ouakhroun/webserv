#ifndef SERVER_HPP
#define SERVER_HPP

#include "Request.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#define SA 		struct sockaddr
#define SA_IN 	struct sockaddr_in
#define PORT 8080
#define SIZE 2048

class	Server
{
	public :
		Server();
		~Server();
		// int	errorExit(int status, std::string errorMsg)
		// {
		// 	if (status < 0)
		// 	{
		// 		std::cerr << errorMsg << std::endl;
		// 		exit (1);
		// 	}
		// 	return (status);
		// }
		// std::string readRequest(int &clientFd)
		// {
		// 	std::string	data;
		// 	int			byte = 0;
		// 	int			readbyte = 1;
		// 	char		request[readbyte + 1];

		// 	while(1)
		// 	{
		// 		byte = errorExit(read(clientFd, request, readbyte), "Error : read Faile!.\n");
		// 		request[byte] = 0;
		// 		data += request;
		// 		if(data.find("\r\n\r\n") != std::string::npos)
		// 			break;
		// 	}
		// 	return (data);
		// }

	private :
		int			servFd;
		int			clientFd;
		// char 		request[SIZE];

		SA_IN		servAddr;

};
#endif
