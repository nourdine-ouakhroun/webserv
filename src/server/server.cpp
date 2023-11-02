#include"server.hpp"

ServerRun::ServerRun()
{
	socketfd = socket(PF_INET, SOCK_STREAM, 0);
	if(socketfd < 0)
	{
		std::cout << "error : socket conection not open ." << std::endl;
		exit(1);
	}
	std::cout << socketfd << std::endl;
		
}
void ServerRun::bindConection(int Port)
{
	bzero(&addressSocketStruct, sizeof(addressSocketStruct));
	addressSocketStruct.sin_family = PF_INET;
	addressSocketStruct.sin_port = htons(Port);
	addressSocketStruct.sin_addr.s_addr = inet_addr("10.11.2.4");

	int bindr =  bind(socketfd,	(struct sockaddr *)&addressSocketStruct, sizeof(addressSocketStruct));
	if(bindr < 0)
	{
		std::cout << "error : the conectin not binded ." << std::endl;
		exit(0);
	}
}

void ServerRun::acceptRquist()
{
}

void ServerRun::listenSocket()
{
	int lestenr = listen(socketfd, 10);
	if(lestenr < 0)
	{
		std::cout << "error : listen" << std::endl;
		exit(0);
	}
}
ServerRun::~ServerRun()
{
	close(socketfd);
	close(newfd);
}
