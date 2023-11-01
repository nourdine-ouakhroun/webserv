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
	char req[2024];
	fd_set cur_sock, ready_sock;
	FD_ZERO(&cur_sock);
	FD_SET(socketfd, &cur_sock);

	while(1)
	{
		FD_SET(socketfd, &cur_sock);
		ready_sock = cur_sock;
		if(select(1024, &ready_sock, NULL, NULL, NULL) < 0)
		{
			std::cout << "error : select" << std::endl;
			exit(0);
		}
		for(int i = 0; i < FD_SETSIZE; i++)
		{
			if(FD_ISSET(i, &ready_sock))
			{
				if(i == socketfd)
				{
					newfd = accept(socketfd, (struct sockaddr *)NULL, NULL);
					FD_SET(newfd, &cur_sock);
					break;
				}
				else
				{
					int bytes = read(i, req, 2024);	
					if(bytes < 0)
					{
						std::cout << "error : read failed ." << std::endl;
						exit(0);
					}
					std::cout << req << std::endl;
					FD_CLR(i, &cur_sock);
					close(i);
				}
			}
		}
	}
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
