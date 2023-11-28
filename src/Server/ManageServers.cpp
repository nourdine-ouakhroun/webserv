#include"ManageServers.hpp"

ManageServers::ManageServers(ServerData	srvers)
{
	this->servers = srvers;
}


void ManageServers::runAllServers()
{
	std::vector <ServerModel> allservers =  servers.getAllServers();
	std::vector<int> allport;
	for (size_t i = 0; i < allservers.size(); i++)
	{
		std::vector <Data> ports = allservers[i].getData("listen");
		for (size_t j = 0; j < ports.size(); j++)
		{
			allport.push_back(static_cast<int>(strtol(ports[j].getValue().c_str(),NULL, 10)));
		}
	}
	for(size_t i = 0; i < allport.size();i++)
	{
		int fd;
		try{fd = Server::setSocket();}
		catch(std::runtime_error &e){std::cout << e.what() << std::endl;}
		try{Server::bindSocket(fd, allport[i]);}
		catch(std::runtime_error &e){std::cout << e.what() << std::endl; continue;}
		try{Server::listenPort(fd); }
		catch(std::runtime_error &e){std::cout << e.what() << std::endl;continue;}
		fdSockets.push_back(fd);
	}
	if(fdSockets.empty())
	{
		int fd;
		try{fd = Server::setSocket();}
		catch(std::runtime_error &e){std::cout << e.what() << std::endl;}
		try{Server::bindSocket(fd, 80);}
		catch(std::runtime_error &e){std::cout << e.what() << std::endl;}
		try{Server::listenPort(fd);}
		catch(std::runtime_error &e){std::cout << e.what() << std::endl;}
		fdSockets.push_back(fd);
	}
	
}
void erase(std::vector<pollfd> & struct_fdsS, size_t j)
{
	std::vector<struct pollfd> returnFds;
	for (size_t i = 0; i < struct_fdsS.size(); i++)
	{
		if(i != j)
			returnFds.push_back(struct_fdsS[i]);
	}
	struct_fdsS = returnFds;
}

std::string readRequist(int fd)
{
	ssize_t		bytes;
	std::string	ContentRequist;
	std::string	boundary;
	char		req[2025];
	bytes = 0;
	ContentRequist.clear();
	bzero(req, 2025);
	int check = 0;
	while(true)
	{
		bzero(req, 2025);
		bytes = recv(fd, req, 2024, 0);
		if(bytes == 0)
			break;
		ContentRequist.append(req);
		size_t pos;
		if((pos = ContentRequist.find("boundary=", 0)) != SIZE_T_MAX && check != 1)
		{
			check = 1;
			boundary = ContentRequist.substr(pos + 9, ContentRequist.find("\r\n", (pos + 9)) - (pos + 9));
		}
		if(check && ContentRequist.find( "--" + boundary + "--") != SIZE_T_MAX)
			check = 0;
		if(bytes < 0 && check == 0)
			break;
		
	}
	if(bytes < 0 && !ContentRequist.size())
		throw std::runtime_error("");
	std::cout << ContentRequist << std::endl;
	return "HTTP/1.1 200 OK\r\n\r\n <h1> hello </h1>";
}
void ManageServers::handler(std::vector<pollfd> &working, std::vector<pollfd> &master, size_t i)
{
	if(working[i].revents & POLLIN || working[i].revents & POLLOUT)
	{
		for (size_t j = 0; j < fdSockets.size(); j++)
		{
			if(working[i].fd == fdSockets[j])
			{
				int newfd = accept(fdSockets[j], NULL, NULL);
				if(newfd < 0)
					throw std::runtime_error("accept : filed!");
				/**
				 * @attention check fcntl fhm chno katakhd mziaaaaan
				*/
				fcntl(newfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
				pollfd fd;
				fd.fd = newfd;
				fd.events = POLLIN | POLLOUT;
				master.push_back(fd);
				return;
			}
		}
		std::string respond;
		try{respond = readRequist(working[i].fd);}
		catch(...){return ;};
		write(working[i].fd, respond.c_str(), respond.size());
		close(working[i].fd);
		erase(master,i);
	}
}
void ManageServers::acceptConection()
{
	std::vector<pollfd> master;
	for (size_t i = 0; i < fdSockets.size(); i++)
	{
		pollfd fd;
		fd.fd = fdSockets[i];
		fd.events = POLLIN | POLLOUT;
		master.push_back(fd);
	}
	while (true)
	{
		std::vector<pollfd> working = master;
		int pint = poll(&working[0], static_cast<nfds_t>(working.size()), 6000);
		if(pint == 0)
		{
			Logger::info(std::cout, "Server ", "reload");
			continue;
		}
		if(pint < 0)
			throw std::runtime_error("poll : poll was filed");
		for (size_t i = 0; i < working.size(); i++)
		{
			try{handler(working, master, i);}
			catch(std::runtime_error &e){std::cout << e.what() << std::endl;}
		}
		
	}
	
}
ManageServers::~ManageServers()
{
}