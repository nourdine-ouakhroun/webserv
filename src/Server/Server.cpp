#include"Server.hpp"


void	Server::bindSocket(int fd, int port)
{
	S_address socket_info;
	socket_info.sin_family = PF_INET;
	socket_info.sin_port = htons(port);
	socket_info.sin_addr.s_addr = INADDR_ANY;
	if(bind(fd, (const sockaddr*)&socket_info, sizeof(socket_info)) == -1)
	{
		close(fd);
		throw std::runtime_error("bind : can't Bind this Port");
	}

}
void	Server::listenPort(int Socketfd)
{
	if(listen(Socketfd, BACKLOG)  == -1)
		throw std::runtime_error("listen : can't listen");
}
int		Server::setSocket()
{
	int fdSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(fdSocket < -1)
		throw std::runtime_error("socket : can't open the file conection");
	return fdSocket;
}
