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
	if(bytes > 0)
	{
		std::string reqtmp(req, (size_t)bytes);
		reqtmp.insert(0, file.rest);
		if(file.requist.empty())
		{
			file.contenlenght = 0;
			file.lenght = 0;
			size_t pos;
			pos = reqtmp.find("\r\n\r\n");
			if(pos == SIZE_T_MAX)
				pos = reqtmp.size();
			else
				pos+=4;
			file.filtred = reqtmp.substr(0 , pos);
			file.rest = reqtmp.substr(pos);
			file.requist = file.filtred;
			pos = reqtmp.find("boundary=");
			file.boundery = pos != SIZE_T_MAX ? reqtmp.substr(pos + 9, reqtmp.find("\r\n", pos + 9) - (pos + 9)) : "";
			pos = reqtmp.find("Content-Length: ");
			file.lenght = file.rest.size();
			if(pos != SIZE_T_MAX)
				file.contenlenght = (size_t)strtol(file.requist.substr(pos + 16, file.requist.find("\r\n", pos)).c_str(), NULL, 10);
			// file.filtred.clear();
		}
		else
		{
			size_t pos;
			pos = reqtmp.find_last_of("\r\n");
			if(pos == SIZE_T_MAX)
				pos = reqtmp.size();
			else
				pos++;
			file.filtred = reqtmp.substr(0 , pos);
			file.rest = reqtmp.substr(pos);
			if(file.boundery.empty() == false)
			{
				size_t pos;
				if(file.status == true && (pos = file.filtred.find(file.boundery)) != SIZE_T_MAX)
				{
						write(file.fd, file.filtred.c_str(), file.filtred.size());
					file.status = false;
					close(file.fd);
					std::cout << "close" << std::endl;
				}
				else if((pos = file.filtred.find("filename=")) != SIZE_T_MAX || file.status == true)
				{
					if(file.status == false)
					{
						// std::cout << file.filtred.substr(pos + 10, file.filtred.find("\r\n", pos) - (pos + 10)) << std::endl;
						file.fd = open(file.filtred.substr(pos + 10, file.filtred.find("\r\n", pos) - (pos + 10)).c_str(), O_CREAT ,O_WRONLY | 0777);
						file.filtred.erase(0, file.filtred.find("\r\n\r\n", pos) + 4);
						std::cout << file.filtred << std::endl;
						write(file.fd, file.filtred.c_str(), file.filtred.size());
					}
					else
					{
						std::cout << file.filtred << std::endl;
						write(file.fd, file.filtred.c_str(), file.filtred.size());
					}
					file.status = true;
				}
			}
			file.lenght += file.filtred.size();
		}
	}
	if(file.lenght != file.contenlenght || file.requist.empty())
		throw std::runtime_error("");
			// std::cout << "[" << file.boundery << "]" << std::endl;

					// std::cout << file.filtred << std::endl;
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