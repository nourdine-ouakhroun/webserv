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

std::string readRequist(FileDepandenc &file)
{
	ssize_t		bytes;
	String	boundary;
	bytes = 0;
	char		req[2025];

	memset(req, 0, 2025);
	bytes = recv(file.fdpoll.fd, req, 2024, 0);
	if(bytes > 0 || !file.filtred.empty())
	{
		std::string request;
		if(bytes > 0)
			request = std::string(req, (size_t)bytes);
		request.insert(0, file.filtred);
		if(file.requist.empty())
		{
			// std::cout << request << std::endl;

			file.contenlenght = 0;
			file.lenght = 0;
			size_t pos;
			pos = request.find("\r\n\r\n");
			if(pos == SIZE_T_MAX)
				pos = request.size();
			else
				pos += 4;
			file.requist = request.substr(0 , pos);
			file.filtred = request.substr(pos);
			pos = request.find("boundary=");
			file.boundary = pos != SIZE_T_MAX ? request.substr(pos + 9, request.find("\r\n", pos + 9) - (pos + 9)) : "";
			pos = request.find("Content-Length: ");
			if(pos != SIZE_T_MAX)
				file.contenlenght = (size_t)strtol(file.requist.substr(pos + 16, file.requist.find("\r\n", pos)).c_str(), NULL, 10);
			file.lenght += file.filtred.size();
		}
		else
		{
			// std::cout << "fddf" << std::endl;
			file.filtred.clear();
			size_t pos;
			pos = request.find_last_of("\r\n");
			// std::cout << pos << std::endl;
			if(pos == SIZE_T_MAX)
				pos = request.size();
			else
				pos++;
			file.filtred = request.substr(0 , pos);
			if(file.boundary.empty() == false)
			{
				size_t pos;
				if(file.status == true && (pos = file.filtred.find(file.boundary)) != SIZE_T_MAX)
				{
					std::string tmpstrig(file.filtred.substr(0, file.filtred.find_last_of("\r\n",pos) - 1));
					write(file.fd, tmpstrig.c_str(), tmpstrig.size());
					file.status = false;
					close(file.fd);
					file.filtred.erase(0, file.filtred.find("\r\n\r\n", pos) + 4);
					std::cout << "close" << std::endl;
				}
				else if((pos = file.filtred.find("filename=")) != SIZE_T_MAX || file.status == true)
				{
					if(file.status == false)
					{
						std::cout << "open" << std::endl;
						size_t end = file.filtred.find("\r\n", pos);
						file.fd = open(file.filtred.substr(pos + 10, end - (pos + 10) - 1).c_str(), O_CREAT | O_RDWR | O_APPEND , 0777);
						file.filtred.erase(0, file.filtred.find("\r\n\r\n", pos) + 4);
						write(file.fd, file.filtred.c_str(), file.filtred.size());
					}
					else
					{
						write(file.fd, file.filtred.c_str(), file.filtred.size());
						file.filtred.clear();
					}
					file.status = true;
				}
				else
				{
					file.requist.append(file.filtred);
					file.filtred.clear();
				}
			}
			file.filtred.append(request.substr(pos));
			file.lenght += file.filtred.size();
		
		}

	}
			// std::cout << file.lenght << " : "  << file.contenlenght << " : " << file.filtred.size() << std::endl;

	// std::cout << file.lenght << file.contenlenght << file.requist.empty() << std::endl;
	if(file.lenght != file.contenlenght || !file.filtred.empty() || file.requist.empty())
		throw std::runtime_error("");
	// std::cout << file.requist << std::endl;
	
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
				master.push_back(tmp);
				return;
			}
		}
		std::string respond;
		try{respond = readRequist(master[i]);}
		catch(std::runtime_error){return ;};
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

		// pworking
		
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