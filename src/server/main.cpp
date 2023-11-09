#include"parssingRequist.hpp"

void HandelRequist(struct pollfd *struct_fds ,int i)
{
	const char *str = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>hello world</h1>";
	int				bytes;
	char			req[2024];
	int				newfd;
	string			ContentRequist;
	ParssingRequist	Req;

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
		while((bytes = read(newfd, req, 2024)) != 0)
		{
			ContentRequist.append(req);
			if(bytes < 2024)
				break;
		}
		if(bytes < 0)
			throw std::runtime_error("read was filed");

		Req.TakeRequistContent(ContentRequist);
		std::cout << ContentRequist << std::endl;
		write(newfd,str,strlen(str));
		close(newfd);
	}
}

void acceptRquist( std::vector<int> servers) 
{
	struct pollfd struct_fds[servers.size()];
	memset(struct_fds, 0 , sizeof(struct_fds));
	for(size_t i = 0; i < servers.size(); i++)
	{
		struct_fds[i].fd = servers[i];
		struct_fds[i].events = POLLOUT | POLLIN;
	}
	while(1)
	{
		int pollr = poll(struct_fds, servers.size(), 6000); 
		if(pollr < 0)
		{
			std::cout << "error : pull" << std::endl;
			exit(1);
		}
		if(pollr == 0)
			continue ;
		for(size_t i = 0; i < servers.size(); i++)
			HandelRequist(struct_fds, i);
	}
}
int main()
{
	std::vector<int> ports;
	ports.push_back(4040);
	ports.push_back(4041);
	std::vector<int> servers;
	for(size_t i = 0;i < ports.size();i++)
	{
		ServerRun Server;
		int serverfd = Server.Newsocket();
		std::cout << ports[i] << std::endl;
		Server.bindConection(ports[i], serverfd); 
		Server.listenSocket(serverfd);
		servers.push_back(serverfd);
	}
	acceptRquist( servers );
}
