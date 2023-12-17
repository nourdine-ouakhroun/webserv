#include "ManageServers.hpp"
#include "../Request/Request.hpp"
#include "FileDepandenc.hpp"
#include <sys/stat.h>
// #include "../Request/StatusCode.hpp"
#define READ_SIZE 1000
std::string	readF(const std::string& path)
{
	std::string  content;
	char	buffer[READ_SIZE + 1];

	int fd = open(path.c_str(), O_RDONLY);

	if (fd < 0)
		return (content);
	int i = 0;
	while (1)
	{
		i++;
		ssize_t r = read(fd, buffer, READ_SIZE);
		if (r <= 0)
			break ;
		buffer[r] = 0;
		content.append(buffer, (u_long)r);
		// std::cout << r << " " << i<< std::endl;
		// std::cout << content.length() << std::endl;
	}
	close(fd);
	return (content);
}




std::string extention(const std::string &path)
{
	size_t pos = 0;
	// std::cout << "path = " << path << "\n";
	if ((pos = path.rfind(".")) != std::string::npos) {
		// std::cout << pos << " " << path.length() << std::endl;
		// std::cout << "extention = " << path.substr(pos, path.length() - pos) << "\n";

		return (path.substr(pos, path.length() - pos));
	}
	return ("");
}

std::string	readRequest(int clientFd)
{
	std::string		request;
	ssize_t			byte = 0;
	size_t			readbyte = 1024;
	char			buf[readbyte + 1];

	while(1)
	{
		byte = read(clientFd, buf, readbyte);
		if (byte <= 0 && request.empty())
			throw std::runtime_error("") ;
		buf[byte] = 0;
		request += buf;
		if (byte < (ssize_t)readbyte)
			break ;
	}
	return (request);
}

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
		// std::cout << working[i].fdpoll.fd << std::endl;
		std::string request;
		try
		{
			request = readRequest(working[i].fdpoll.fd);
		}
		catch(const std::exception )
		{
			return ;
		}


		Request req;
		std::string content;
		// std::cout << request << std::endl;
		req.parseRequest(request);
		
		Response res;
		try {
			req.isFormed(res);
			// req.isMatched(res);
			// req.isRedirected(res);
			// req.isAllowed(res);
			// req.whichMethode(res); // GET POST DELETE
			res.send();
		}
		catch (...) {
			res.send();
		}

		// if (req.getMethode() == "GET") {
		// 	std::string path = req.getPathname();
		// 	std::cout << "|" << path << "|" << std::endl;
		// 	struct stat  s;
		// 	if (stat(path.c_str(), &s) == 0 && s.st_mode & S_IFDIR )
		// 	{
		// 		std::cout << "==> isDerctory <==" << std::endl;
		// 	}
		// 	else
		// 	{
		// 		std::cout << "==> isfile <==" << std::endl;
		// 	}
		// }
		// else if (req.getMethode() == "POST") {

		// }
		// else if (req.getMethode() == "DELETE") {

		// }

		// res.

		// maps s = req.getBody();
		// for (maps::iterator i = s.begin(); i != s.end(); i++)
		// {
		// 	std::cerr << i->first << " " << i->second << std::endl ;
		// }
		
		// std::cerr << req.getQuery() << std::endl;

		res.setHeader("server", "dokoko");
		res.setHeader("Content-Type", res.getMimeType(extention(req.getPathname())) );
		std::string a = req.header("Host");
		// std::cout << "|" << a << "|" << std::endl << std::flush;

		// std::cout << "Host " <<  req.header("Host") << "\n";
		std::string	host;
		int			port = 80;
		std::vector<std::string> h = split(a, ":");
		for (size_t i = 0; i < h.size(); i++)
		{
			if (i == 0)
				host = h[i];
			else
				port = std::stoi(h[i]);
		}

		// std::cout << h[0].size() << " " << h[].size() << std::endl;
		// if (!h.empty() && h.size() >= 2) {
		// 	if (!h[0].empty())
		// 		std::cout << "1 - |" << h[0] << "|\n" << std::flush;
		// 	if (!h[1].empty()) 
		// 		std::cout << "2 - |" << h[1] << "|\n" << std::flush;
		// }

		// if (!h.empty())
		// {
		// 	std::cout << h[0].empty() << " " << h[1].empty() << std::endl;
		// 	if (!h[0].empty())
		// 		host = h[0];
		// 	if (!h[1].empty())
		// 		port = std::stoi(h[1]);
		// }
		// std::cout << host << " " << port << std::endl;

		if (req.getPathname() == "/")
		{
			res.setHeader("Content-Type", res.getMimeType(".html"));
			content = readF("/Users/mzeroual/Desktop/webserv/src/Request/web/index.html");

		}
		else
			content = readF("/Users/mzeroual/Desktop/webserv/src/Request/web/" + req.getPathname());


		if (!content.empty())
			res.setBody(content);
		res.makeHeaderResponse();
		res.makeBodyResponse();
		
		// std::cout << "==>" << "/Users/mzeroual/Desktop/webserv/src/Request/web/" + req.getPathname() << "\n" << res.getResponse() << "<=="  << std::endl;
		std::string response = res.getResponse();
		// std::cerr << response << std::endl;
		// write(working[i].fdpoll.fd, response.c_str(), response.length());
		// size_t countwrite = 0;
		// ssize_t byteSend = 0;
		size_t len = response.length();
		size_t len1 = len;
		
		// int ii = -1;
		while (len)
		{
			if (len < READ_SIZE) {
				while(len)
				{
					if (write(working[i].fdpoll.fd, response.c_str() + len1-len, 1) == -1)
					{
						// std::cerr << "write failed!" << std::endl;
						if (read(working[i].fdpoll.fd, 0, 1) <= 0)
							return ;
						continue;
					}
					len--;
				}
			}
			else {
				if (write(working[i].fdpoll.fd, response.c_str() + len1-len, READ_SIZE) == -1)
				{
					if (read(working[i].fdpoll.fd, 0, 1) <= 0)
						return ;
					// std::cerr << "write failed!" << std::endl;
					continue;
				}
				len -= READ_SIZE;
			}
		}

		// ssize_t s = send(working[i].fdpoll.fd, response.c_str() + c, READ_SIZE, 0);
		// if (s == -1)
		// {
		// 	close(working[i].fdpoll.fd);
		// 	return ;
		// }
		// else if (s && s != READ_SIZE)
		// 	break ;
		// c += READ_SIZE;


		// while (len1 > 0)
		// {
		// 	ii++;
		// 	byteSend = write(working[i].fdpoll.fd, response.c_str() + countwrite, READ_SIZE);
		// 	if (byteSend == -1) {
		// 		std::cout << "write error " << ii << std::endl;
		// 		break ;
		// 	}
		// 	// if (countwrite == len) {
		// 	// 	// std::cout << "0" << "\n";
		// 	// 	break ;
		// 	// }
		// 	// else {
		// 	// countwrite += (size_t)byteSend;
		// 	countwrite += READ_SIZE;
		// 	// if (countwrite >= response.length())
		// 		// break;
		// 	len1 -= READ_SIZE;
		// 	std::cout << len1 - READ_SIZE << std::endl;
		// 	// std::cout << countwrite << " " << byteSend << " "  << READ_SIZE << std::endl;
		// 	// std::cerr << byteSend << " "  << READ_SIZE << std::endl;
		// }
		// std::cout << "----------------------------------------" <<  std::endl;


		// write(working[i].fdpoll.fd, res.getResponse().c_str(), res.getResponse().size());
		close(working[i].fdpoll.fd);
		erase(master, i);
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
		// [8080, 80 , 555]
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
			signal(SIGPIPE, SIG_IGN);
		}

		
	}
	
}
ManageServers::~ManageServers()
{
}