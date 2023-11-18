#include"Server.hpp"

ServerRun::ServerRun(ServerData	smodel)
{
	this->serves = smodel;
}
int	ServerRun::Newsocket()
{
	int socketfd = socket(PF_INET, SOCK_STREAM, 0);
	if(socketfd < 0)
	{
		std::cout << "error : socket conection not open ." << std::endl;
		exit(1);
	}
	int option = 1;
	setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));
	std::cout << socketfd << std::endl;
	return socketfd;
}

String	ServerRun::ParssingRecuistContent(String	ContentRequist)
{
	std::vector <String> spletLines = ParssingRequist::SplitBynewLine(ContentRequist);
	_requset _requisteconten = ParssingRequist::setreq(spletLines);
	return "<h1> hello </h1>";
}

void erase(std::vector<struct pollfd> & struct_fdsS, size_t j)
{
	std::vector<struct pollfd> returnFds;
	for (size_t i = 0; i < struct_fdsS.size(); i++)
	{
		if(i != j)
			returnFds.push_back(struct_fdsS[i]);
	}
	struct_fdsS = returnFds;
}

std::string status(std::string statuscode, std::string stringcode)
{
	(void) statuscode;
	(void) stringcode;
	std::string header = "HTTP/1.1 ";
	header.append(statuscode);
	header.append("\r\nContent-Type: text/html\r\n\r\n");
	header.append(stringcode);
	return header;
}

void	ServerRun::HandelRequist(struct pollfd	*struct_fds ,size_t	i, std::vector<struct pollfd>	&struct_fdsS, std::vector<int> servers)
{
	if (struct_fds[i].revents & POLLOUT || struct_fds[i].revents &  POLLIN)
	{
		for (size_t j = 0; j < servers.size(); j++)
		{
			if(struct_fds[i].fd == servers[j])
			{
				int			newfd;
				newfd = accept(struct_fds[i].fd, (struct sockaddr *)NULL, NULL);
				if( newfd < 0 )
				{
					std::cout << "	error	:	accpet	" << std::endl;	
					exit(0);
				}
				fcntl(newfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
				struct pollfd fd;
				fd.fd = newfd;
				fd.events = POLLOUT | POLLIN;
				struct_fdsS.push_back(fd);
				return;
			}
		}
		ssize_t		bytes;
		std::string	ContentRequist;
		char		req[2];
		bytes = 0;
		ContentRequist.clear();
		bzero(req, 2);
		while((bytes = recv(struct_fds[i].fd, req, 1, 0)) > 0)
		{
			ContentRequist.append(req);
			bzero(req, 2);
		}
		if(bytes < 0 && !ContentRequist.size())
			return;
		std::cout << ContentRequist << std::endl;
		std::string respond = status("200 OK", ParssingRecuistContent(ContentRequist));
		write(struct_fds[i].fd, respond.c_str(), respond.length());
		close(struct_fds[i].fd);
		erase(struct_fdsS,i);
	}
}

void	ServerRun::acceptRquist( std::vector<int>	servers ) 
{
	std::vector<struct pollfd>	struct_fdsS;
	struct pollfd fd;
	int pollValeu;
	for (size_t	i = 0; i < servers.size(); i++)
	{
		fd.fd = servers[i];
		fd.events = POLLOUT | POLLIN;
		struct_fdsS.push_back(fd);
	}
	while (1)
	{
		std::vector<struct pollfd>	struct_fdsC(struct_fdsS);
		pollValeu = poll(&struct_fdsC[0], (unsigned int)struct_fdsC.size(), 6000);
		if (pollValeu < 0)
		{
			std::cout << "error : poll" << std::endl;
			exit(1);
		}
		if (pollValeu == 0)
			continue ;
		for(size_t i = 0; i < struct_fdsC.size(); i++)
			HandelRequist(&struct_fdsC[0], i, struct_fdsS, servers);
	}
}
void	ServerRun::RunAllServers()
{
	std::vector<int> servers;
	std::vector<int> ports;
	std::vector<ServerModel> smodel = serves.getAllServers();
	for(size_t i = 0; i < smodel.size(); i++)
	{
		std::vector<Data> portsValeu = smodel[i].getData("listen");
		for(size_t j = 0; j < portsValeu.size(); j++)
		{
			ports.push_back((int)strtol(portsValeu[j].getValue().c_str(), NULL, 10));
		}
	}
	for(size_t i = 0; i < ports.size(); i++)
	{
		int serverfd = this->Newsocket();
		std::cout << ports[i] << std::endl;
		this->bindConection(ports[i], serverfd); 
		this->listenSocket(serverfd);
		servers.push_back(serverfd);
	}
	this->acceptRquist( servers );
}

void	ServerRun::bindConection(int port, int socketfd)
{
	struct sockaddr_in addressSocketStruct;
	bzero(&addressSocketStruct, sizeof(addressSocketStruct));
	addressSocketStruct.sin_family = PF_INET;
	addressSocketStruct.sin_port = htons(port);
	addressSocketStruct.sin_addr.s_addr = INADDR_ANY;

	int bindr =  bind(socketfd,	(struct sockaddr *)&addressSocketStruct, sizeof(addressSocketStruct));
	if(bindr < 0)
	{
		std::cout << "error : the conectin not binded ." << std::endl;
		exit(0);
	}
}
void ServerRun::listenSocket(int socketfd)
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
	//close(socketfd);
}
