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
		try{
			fd = Server::setSocket();
			Server::bindSocket(fd, allport[i]);
			Server::listenPort(fd);
		}
		catch(std::runtime_error &e){std::cout << e.what() << std::endl;continue;}
		fdSockets.push_back(fd);
	}
	if(fdSockets.empty())
	{
		int fd;
		try{
			fd = Server::setSocket();
			Server::bindSocket(fd, 80);
			Server::listenPort(fd);
		}
		catch(std::runtime_error &e){std::cout << e.what() << std::endl;return;}
		fdSockets.push_back(fd);
	}
	
}
void erase(std::vector<FileDependency> & struct_fdsS, size_t j)
{
	std::vector<FileDependency> returnFds;
	for (size_t i = 0; i < struct_fdsS.size(); i++)
	{
		if(i != j)
			returnFds.push_back(struct_fdsS[i]);
	}
	struct_fdsS = returnFds;
}

int headerMethod(String line)
{
	std::vector<String> splited = line.split();
	if(splited[0] == "GET")
		return 0;
	if(splited[0] == "POST")
		return 1;
	if(splited[0] == "DELET")
		return 2;
	return -1;
}

std::string readRequist(FileDependency &file)
{
	ssize_t		bytes;
	String	boundary;
	bytes = 0;
	char		req[2025];

	memset(req, 0, 2025);
	bytes = recv(file.getFdPoll().fd, req, 2024, 0);
	std::string request;
	if(bytes > 0)
	{
		request = std::string(req, (size_t)bytes);
		if(file.rest.empty() == false)
			request.insert(0, file.rest);
		;
		if(file.requist.empty())
		{
			method = headerMethod(request.substr(0, request.find("\r\n")));
		}
	}
	if(file.lenght != file.contenlenght || !file.rest.empty() || file.requist.empty())
		throw std::runtime_error("");

	return "HTTP/1.1 200 OK\r\n\r\n <h1> hello </h1>";
}
void ManageServers::handler(std::vector<FileDependency> &working, std::vector<FileDependency> &master, size_t i)
{
	for (size_t j = 0; j < fdSockets.size(); j++)
	{
		if(working[i].getFdPoll().fd == fdSockets[j])
		{
			int newfd = accept(fdSockets[j], NULL, NULL);
			if(newfd < 0)
				throw std::runtime_error("accept : filed!");
			/**
			 * @attention check fcntl fhm chno katakhd mziaaaaan
			*/
			fcntl(newfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
			FileDependency tmp;
			tmp.setFdPoll(fdSockets[i], POLLOUT);
			master.push_back(tmp);
			return;
		}
	}
	std::string respond;
	try{respond = readRequist(master[i]);}
	catch(std::runtime_error){return ;};
	write(working[i].getFdPoll().fd, respond.c_str(), respond.size());
	std::cout << "hiiiii" << std::endl;
	close(working[i].getFdPoll().fd);
	erase(master,i);
}
void ManageServers::acceptConection()
{
	std::vector<FileDependency> master;
	for (size_t i = 0; i < fdSockets.size(); i++)
	{
		FileDependency tmp;
		tmp.setFdPoll(fdSockets[i], POLLOUT);
		master.push_back(tmp);
	}
	while (true)
	{
		std::vector<FileDependency> working = master;
		std::vector<pollfd> pworking;
		for (size_t i = 0; i < working.size(); i++)
		{
			pworking.push_back(working[i].getFdPoll());
		}
		int pint = poll(&pworking[0], static_cast<nfds_t>(pworking.size()), 6000);
		for (size_t i = 0; i < pworking.size(); i++)
		{
			working[i].setFdPoll(pworking[i]);
		}
		if(pint == 0)
		{
			Logger::info(std::cout, "Server ", "reload");
			continue;
		}
		if(pint < 0)
			throw std::runtime_error("poll : poll was failed");
		for (size_t i = 0; i < working.size(); i++)
		{
			if(working[i].getFdPoll().revents & POLLIN || working[i].getFdPoll().revents & POLLOUT)
			{
				try{handler(working, master, i);}
				catch(std::runtime_error &e){std::cout << e.what() << std::endl;}
			}
		}
		
	}
	
}
ManageServers::~ManageServers()
{
}