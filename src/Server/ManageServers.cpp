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
	bytes = 0;
	ContentRequist.clear();
	// int check = 0;
	while(true)
	{
		char req[2025];
		bytes = recv(fd, req, 2024, 0);
		if(bytes <= 0)
		{
			break;
		}
		// std::cout << req << std::endl;
		ContentRequist.append(req);
		// std::cout << ContentRequist << std::endl;
		// size_t pos;
		// if((pos = ContentRequist.find("boundary=", 0)) != SIZE_T_MAX && check != 1)
		// {
		// 	check = 1;
		// 	boundary = ContentRequist.substr(pos + 9, ContentRequist.find("\r\n", (pos + 9)) - (pos + 9));
		// }
		// if(check && ContentRequist.find( "--" + boundary + "--") != SIZE_T_MAX)
		// 	check = 0;
		// if(bytes < 0 && check == 0)
		// 	break;	
	}
	if(bytes < 0 && !ContentRequist.size())
		throw std::runtime_error("");
	std::cout << ContentRequist << std::endl;
	return "HTTP/1.1 200 OK\r\n\r\n <h1> hello </h1>";
}
void ManageServers::handler(std::vector<FileDepandenc> &working, std::vector<FileDepandenc> &master, size_t i)
{
	if(working[i].getFd().revents & POLLIN || working[i].getFd().revents & POLLOUT)
	{
		for (size_t j = 0; j < fdSockets.size(); j++)
		{
			if(working[i].getFd().fd == fdSockets[j])
			{
				int newfd = accept(fdSockets[j], NULL, NULL);
				if(newfd < 0)
					throw std::runtime_error("accept : filed!");
				/**
				 * @attention check fcntl fhm chno katakhd mziaaaaan
				*/
				fcntl(newfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
				FileDepandenc fdfile;
				pollfd fd;
				fd.fd = newfd;
				fd.events = POLLIN | POLLOUT;
				fdfile.setFd(fd);
				master.push_back(fdfile);
				return;
			}
		}
		std::string respond;
		try{respond = readRequist(working[i].getFd().fd);}
		catch(...){return ;};
		write(working[i].getFd().fd, respond.c_str(), respond.size());
		// std::cout << "dddddddddddddd" << std::endl;
		// close(working[i].fd);
		// erase(master,i);
	}
}
void ManageServers::acceptConection()
{
	std::vector<FileDepandenc> master;
	for (size_t i = 0; i < fdSockets.size(); i++)
	{
		FileDepandenc fdfile;
		pollfd fd;
		fd.fd = fdSockets[i];
		fd.events = POLLIN | POLLOUT;
		fdfile.setFd(fd);
		master.push_back(fdfile);
	}
	while (true)
	{
		std::vector<FileDepandenc> working = master;
		int pint = poll(&working[0].getFd(), static_cast<nfds_t>(working.size()), 6000);
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