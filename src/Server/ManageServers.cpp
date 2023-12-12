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
	if(line.empty() == true)
		throw std::runtime_error("");
	std::vector<String> splited = line.split();
	if(splited[0] == "GET")
		return 0;
	if(splited[0] == "POST")
		return 1;
	if(splited[0] == "DELET")
		return 2;
	return 3;
}


void methodSerch(FileDependency &file, std::string request)
{
	file.rest = request;
	size_t pos = request.find("\r\n");
	if(pos != NPOS)
		file.setMethod(headerMethod(request.substr(0, pos)));
	throw std::runtime_error("");
}

void setOnlyHeadre(FileDependency &file, std::string request)
{
	file.rest = request;
	size_t	pos = request.find("\r\n\r\n");
	if(pos == NPOS)
		throw std::runtime_error("");
	file.setRequist(request.substr(0, pos + 4), pos + 4);
	file.rest = request.substr(pos + 4);
	if(file.getMethod() == POST)
	{
		size_t	pos = file.getRequist().find("Content-Length: ");
		if(pos == NPOS)
			return ;
		long lenght = strtol(file.getRequist().substr(pos + 16).c_str(), NULL, 10);
		file.setContenlenght(static_cast<size_t>(lenght));
		pos = file.getRequist().find("boundary=");
		if(pos != NPOS)
		{
			std::string boundary = file.getRequist().substr(pos + 9, file.getRequist().find("\r\n", pos));
			file.setBoundary(boundary);
		}
	}
}

std::string readRequist(FileDependency &file)
{
	ssize_t		bytes;
	String		boundary;
	char		buffer[READ_NUMBER];

	bytes = 0;
	memset(buffer, 0, READ_NUMBER);
	bytes = recv(file.getFdPoll().fd, buffer, READ_NUMBER - 1, 0);

	if(bytes > 0 || file.rest.empty() == false)
	{
		if(bytes < 0)
			bytes = 0;
		std::string request (buffer, (size_t)bytes);
		request.insert(0, file.rest);
		// std::cout << request << std::endl;
		if(file.getMethod() == -1)
			methodSerch(file, request);
		else if(file.getRequist().empty() == true)
			setOnlyHeadre(file, request);
		else if(file.getMethod() == POST)
		{
			if(file.getBoundary().empty() == false)
			{
				size_t pos = request.find("Content-Disposition: form-data; name=");
				if(pos != NPOS)
				{
					pos = request.find("filename=\"", pos);
					size_t end = request.find("\"", pos + 10);
					std::string filename(request.substr(pos + 10, end - (pos + 10)));
					int fd = open(filename.c_str(), O_CREAT | O_APPEND, 0777);
				}
			}
		}
	}

	if(file.getLenght() != file.getContenlenght() || !file.rest.empty() || file.getRequist().empty())
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
			 * @todo check fcntl fhm chno katakhd mziaaaaan
			*/
			fcntl(newfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
			FileDependency tmp;
			tmp.setFdPoll(newfd, POLLOUT | POLL_IN);
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
		tmp.setFdPoll(fdSockets[i], POLL_IN);
		master.push_back(tmp);
	}
	while (true)
	{
		std::vector<FileDependency> working = master;
		std::vector<pollfd> pworking;
		for (size_t i = 0; i < working.size(); i++)
			pworking.push_back(working[i].getFdPoll());
		int pint = poll(&pworking[0], static_cast<nfds_t>(pworking.size()), 6000);
		for (size_t i = 0; i < pworking.size(); i++)
			working[i].setFdPoll(pworking[i]);
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