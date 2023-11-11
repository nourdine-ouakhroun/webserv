#include"Server.hpp"

ServerRun::ServerRun(ServerData smodel)
{
	this->serves = smodel;
}
int ServerRun::Newsocket()
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

void ServerRun::ParssingRecuistContent(std::string ContentRequist)
{
	ParssingRequist	Req(ContentRequist);
	std::vector<std::string>	RequistContentASplite;
	std::vector<Data> _data;
	RequistContentASplite	=	Req.SplitBynewLine();
	
	for(size_t i = 1; i < RequistContentASplite.size(); i++)
	{
		try {_data.push_back(Req.SpliteEvryLine(RequistContentASplite.at(i)));}
		catch(const std::exception& e){}
	}
	String str(RequistContentASplite[0]);
	std::cout << str.split().at(1) << std::endl;
	// for (size_t i = 0; i < _data.size(); i++)
	// {
	// 	std::cout <<  "key \t[==>" << _data.at(i).getKey() << "<==]";
	// 	std::cout <<  "valeu\t[==>" << _data.at(i).getValue() << "<==]" << std::endl;
	// }



	std::string	valeu;
	size_t		position;

	position = valeu.find(':');
	valeu = _data.at(0).getValue();

	std::vector<ServerModel> smodel = serves.getServersByPort((unsigned short)strtol(valeu.substr(position + 1).c_str(), NULL, 10));

	// ServerModel::printServerModelInfo(smodel[0]);
	// std::vector<Data> ports = smodel.getData("listen");
}
void ServerRun::HandelRequist(struct pollfd	*struct_fds ,size_t	i)
{
	std::string str = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>tl9na sali dakchi</h1>";
	ssize_t				bytes;
	char				req[2024];
	int					newfd;
	std::string			ContentRequist;

	if(struct_fds[i].revents == POLLOUT || struct_fds[i].revents ==  POLLIN)
	{
		ContentRequist.clear();
		bytes = 0;
		newfd = accept(struct_fds[i].fd, (struct sockaddr *)NULL, NULL);

		if(newfd < 0)
		{
			std::cout << "error : accpet" << std::endl;	
			exit(0);
		}
		
		bzero(req, 2024);
		while((bytes = recv(newfd, req, 2023, 0)) != 0)
		{
			ContentRequist.append(req);
			bzero(req, 2024);
			if (bytes < 2023)
				break;
		}
		if(bytes < 0)
			throw std::runtime_error("read was filed");
		ParssingRecuistContent(ContentRequist);
		write(newfd,str.c_str(),strlen(str.c_str()));
		close(newfd);
	}
}

void ServerRun::acceptRquist( std::vector<int> servers ) 
{
	struct pollfd	struct_fds[servers.size()];
	int pollValeu;
	memset(struct_fds, 0 , sizeof(struct_fds));
	for (size_t	i = 0; i < servers.size(); i++)
	{
		struct_fds[i].fd	 =	servers[i];
		struct_fds[i].events =	POLLOUT | POLLIN;
	}
	while (1)
	{
		pollValeu = poll(struct_fds, (unsigned int)servers.size(), 6000); 
		if (pollValeu < 0)
		{
			std::cout << "error : pull" << std::endl;
			exit(1);
		}
		if (pollValeu == 0)
			continue ;
		for(size_t i = 0; i < servers.size(); i++)
			HandelRequist(struct_fds, i);
	}
}
void ServerRun::RunAllServers()
{
	std::vector<int> servers;
	ServerModel smodel = serves.getDefaultServer();
	std::vector<Data> ports = smodel.getData("listen");
	for(size_t i = 0;i < ports.size();i++)
	{
		int serverfd = this->Newsocket();
		std::cout << ports[i].getValue() << std::endl;
		this->bindConection((int)strtol(ports[i].getValue().c_str(), NULL, 10), serverfd); 
		this->listenSocket(serverfd);
		servers.push_back(serverfd);
	}
	this->acceptRquist( servers );
}
void ServerRun::bindConection(int port, int socketfd)
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
