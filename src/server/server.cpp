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

ServerRun::~ServerRun()
{
	close(socketfd);
	close(newfd);
}
