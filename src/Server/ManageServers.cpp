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
void erase(std::vector<FileDepandenc> & struct_fdsS, size_t j)
{
	std::vector<FileDepandenc> returnFds;
	for (size_t i = 0; i < struct_fdsS.size(); i++)
	{
		if(i != j)
			returnFds.push_back(struct_fdsS[i]);
	}
	struct_fdsS = returnFds;
}
// static size_t k;

std::string readRequist(FileDepandenc &file)
{
	ssize_t		bytes;
	std::string	boundary;
	bytes = 0;
	char		req[2025];
	while(true)
	{
		memset(req, 0, 2025);
		bytes = recv(file.fdpoll.fd, req, 2024, 0);
		// if(k >= 5)
		// 	exit(1);
		if(bytes <= 0)
			break;
		std::string reqtmp = req;
		reqtmp.insert(0, file.rest);
		size_t pos;
		// std::cout << "(" << req << ")"<< std::endl;
		pos = reqtmp.find_last_of("\r\n");
		if(pos != SIZE_T_MAX)
			pos = reqtmp.size() - 1;
		std::string filtred = reqtmp.substr(0 , pos + 1);
		std::string resttmp = reqtmp.substr(pos + 1 , reqtmp.size() - pos + 1);
		std::cout << "[" << resttmp << "]"<< std::endl;
		file.requist.append(filtred, file.requist.size(), filtred.size());
		file.rest = resttmp;

		if(file.getStatus() == 0 && file.boundery.empty() && (pos = filtred.find("boundary=", 0)) != SIZE_T_MAX)
		{
			file.boundery = filtred.substr(pos + 9, filtred.find("\r\n", (pos + 9)) - (pos + 9));
			file.setStatus(1);
		}
		// if(!file.header.empty() && (pos = file.getRequist().find("\r\n\r\n", 0)) != SIZE_T_MAX)
		// {
		// 	std::string content = file.getRequist().substr(0 , pos);
		// 	if((pos = content.find("filename=")) != SIZE_T_MAX)
		// 		std::cout << content.substr(pos + 9, content.find("\r\n" , pos) - (pos + 9));
		// 	exit(1);
		// }
		if(!file.boundery.empty() && file.getStatus() && filtred.find( "--" + file.boundery + "--" ) != SIZE_T_MAX)
		{
			file.setStatus(0);
		}
	}
	if(file.getStatus() || file.requist.empty())
		throw std::runtime_error("");

	return "HTTP/1.1 200 OK\r\n\r\n <h1> hello </h1>";
}
void ManageServers::handler(std::vector<FileDepandenc> &working, std::vector<FileDepandenc> &master, size_t i)
{
	if(working[i].fdpoll.revents & POLLIN || working[i].fdpoll.revents & POLLOUT)
	{
		for (size_t j = 0; j < fdSockets.size(); j++)
		{
			if(working[i].fdpoll.fd == fdSockets[j])
			{
				int newfd = accept(fdSockets[j], NULL, NULL);
				if(newfd < 0)
					throw std::runtime_error("accept : filed!");
				/**
				 * @attention check fcntl fhm chno katakhd mziaaaaan
				*/
				fcntl(newfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
				FileDepandenc tmp;
				pollfd fd;
				fd.fd = newfd;
				fd.events = POLLIN | POLLOUT;
				tmp.fdpoll = fd;
				tmp.setStatus(0);
				master.push_back(tmp);
				return;
			}
		}
		std::string respond;
		try{respond = readRequist(master[i]);}
		catch(...){return ;};
		write(working[i].fdpoll.fd, respond.c_str(), respond.size());
		std::cout << "hiiiii" << std::endl;
		close(working[i].fdpoll.fd);
		erase(master,i);
	}
}
void ManageServers::acceptConection()
{
	std::vector<FileDepandenc> master;
	for (size_t i = 0; i < fdSockets.size(); i++)
	{
		FileDepandenc tmp;
		pollfd fd;
		fd.fd = fdSockets[i];
		fd.events = POLLIN | POLLOUT;
		tmp.fdpoll = fd;
		tmp.setStatus(0);
		master.push_back(tmp);
	}
	while (true)
	{
		std::vector<FileDepandenc> working = master;
		std::vector<pollfd> pworking;
		for (size_t i = 0; i < working.size(); i++)
		{
			pworking.push_back(working[i].fdpoll);
		}
		
		int pint = poll(&pworking[0], static_cast<nfds_t>(pworking.size()), 6000);
		for (size_t i = 0; i < pworking.size(); i++)
		{
			working[i].fdpoll = pworking[i];
		}
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