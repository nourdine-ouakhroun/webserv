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
	file.setRequist(request.substr(0, pos + 4));
	// file.setRequist(request.substr(0, pos + 4), pos + 4);
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
			std::string boundary = file.getRequist().substr(pos + 9, file.getRequist().find("\r\n", pos) - (pos + 9));
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

		if(file.getMethod() == -1)
			methodSerch(file, request);
		else if(file.getRequist().empty() == true)
			setOnlyHeadre(file, request);
		else if(file.getMethod() == POST)
		{

			if(file.getBoundary().empty() == false)
			{
				// std::cout << request << std::endl;
				// exit(1);
				file.rest = request;
				size_t begin;
				if(file.status == PUTINSTRING)
				{
					if((begin = file.rest.find(file.getBoundary())) != NPOS)
					{
						size_t	end = file.rest.find_last_of("\r\n", begin) + 1;
						file.setRequist(file.rest.substr(0, end));
						file.setLenght(end);
						file.rest.erase(0, end);
						file.status = DEFAULT;
					}
					throw std::runtime_error("");
				}
				if(file.status == PUTINFILE)
				{
					if((begin = file.rest.find(file.getBoundary())) != NPOS)
					{
						size_t	end = file.rest.find_last_of("\r\n", begin);
						if(end == NPOS)
							end = 0;
						else
							end -= 1 ;
						file.setLenght(end);
						write(file.getFd(), file.rest.substr(0, end).c_str(), file.rest.substr(0, end).size());
						close(file.getFd());
						file.rest.erase(0, end);
						file.status = DEFAULT;
						throw std::runtime_error("");
					}
					size_t	end = file.rest.find_last_of("\r\n") + 1;
					write(file.getFd(), file.rest.substr(0, end).c_str(), file.rest.substr(0, end).size());
					file.setLenght(end);
					file.rest.erase(0, end);
					throw std::runtime_error("");
				}
				if(file.rest.find("--" + file.getBoundary() + "--") != NPOS)
				{
					file.setLenght(file.rest.size());
					file.setRequist(file.rest);
					file.rest.erase(0, file.rest.size());
					throw std::runtime_error("");
				}
				if(file.status == DEFAULT && (begin = file.rest.find(file.getBoundary()) != NPOS))
				{
					begin = file.rest.find_last_of("\r\n", begin);
					if(begin == NPOS)
						begin = 0;
					else
						begin += 2;
					size_t	end = file.rest.find("\r\n\r\n", begin);
					if(end == NPOS)
						throw std::runtime_error("");
					std::string tmp_string(file.rest.substr(begin,  (end + 4) - begin));
					file.rest.erase(begin, (end + 4) - begin);
					file.setLenght(((end + 4) - begin));
					size_t	pos = tmp_string.find("filename=\"", begin);
					if(pos != NPOS)
					{
						std::string filename(tmp_string.substr(pos + 10, tmp_string.find("\"", pos + 10) - (pos + 10)));
						int	fd = open(filename.c_str(), O_CREAT | O_RDWR | O_APPEND, 0777);
						file.setFd(fd);
						file.status = PUTINFILE;
						throw std::runtime_error("");
					}
					file.status = PUTINSTRING;
					file.setRequist(tmp_string);
					// file.setRequist(tmp_string, tmp_string.size());
					throw std::runtime_error("");
				}
			}
		}
	}
	if(file.getLenght() != file.getContenlenght() || !file.rest.empty() || file.getRequist().empty())
		throw std::runtime_error("");
	std::cout << file.getRequist() << std::endl;
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