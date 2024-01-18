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


		Request		req;
		Response	res;
		req.parseRequest(request);

		// std::cout << request << std::endl;
		// std::cout << req.getPathname() << std::endl;



		// std::string content;
		std::string	fileToServe;
		try {
			
			ServerModel	server = ServerData::getServer(this->servers, req.header("Host"))[0];
			fileToServe = req.checkServer(server);

			res.setBody(readF(fileToServe));
		}
		catch (int status) {
			if (status == 404) {
				res.setStatusCode(status);
				res.setMsg("Not Found");
				res.setBody("<h1>Page Not Found</h1>");
				// std::cout << "Not found" << std::endl;
			}
			else if (status == 403) {
				res.setStatusCode(status);
				res.setMsg("Page Is Forbidden");
				res.setBody("<h1>Page Is Forbidden</h1>");
				// forbident
			}
			else if (status == 500) {
				res.setStatusCode(status);
				res.setMsg("Not Implamented");
				res.setBody("<h1>Not Implamented</h1>");
			}
			else if (status == 400) {
				res.setStatusCode(status);
				res.setMsg("Bad Request");
				res.setBody("<h1>Bad Request</h1>");
			}
			else if (status == 414) {
				res.setStatusCode(status);
				res.setMsg("Request-Uri Too Longe");
				res.setBody("<h1>Request-Uri Too Longe<h1>");
			}
			else if (status == 413) {
				res.setStatusCode(status);
				res.setMsg("Request Entity Too Longe");
				res.setBody("<h1>Request Entity Too Longe<h1>");
			}
			else if (status == 301) {
				res.setStatusCode(status);
				res.setMsg("Move Permanently");
				res.setHeader("location", req.getPathname() + "/");
				// res.setBody("<h1>Request Entity Too Longe<h1>");
			}
			// std::cout << status << "Error not found" << std::endl;
			// make Response With Status Code 
			// catch Exepction and make Response Error.
			// res.send(); // Send Response
		}

		std::ifstream fileStream(fileToServe, std::ifstream::ate | std::ifstream::binary);
		std::string	contentLength = std::to_string(fileStream.tellg());
		fileStream.close();

		res.setHeader("server", "nginx-v2");
		res.setHeader("Content-Type", res.getMimeType(extention(fileToServe)) );
		res.setHeader("Content-Length", contentLength);
		res.makeHeaderResponse();
		res.makeBodyResponse();

		std::string response = res.getResponse();
		ssize_t sendByte = 0;
		sendByte = write(working[i].fdpoll.fd, response.c_str(), response.length());
		response.erase(0, (size_t)sendByte);
		
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