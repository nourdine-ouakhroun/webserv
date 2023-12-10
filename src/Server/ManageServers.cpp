#include"ManageServers.hpp"
#include "../Request/Request.hpp"
#include "../Request/Response.hpp"
#include"FileDepandenc.hpp"
// #include "../Request/StatusCode.hpp"

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

// std::string readRequist(FileDepandenc &file)
// {
// 	/**
// 	 * @attention chof rah 3ndk hadchi comantaih caml yak 
// 	 * had function khdm fwst nha dakchi li knti dayr nta
// 	 * ana khlini tanfini lik dik requist ok zwin ylh rah dinmk mongooooool mkhli liya session ma9lti liya chno ndir fiha wach nzid lik chi 7aja wach n9s lik chi7aja mongoooooooooooooool 
// 	*/
// 	ssize_t		bytes;
// 	String	boundary;
// 	bytes = 0;
// 	char		req[2025];

// 	memset(req, 0, 2025);
// 	bytes = recv(file.fdpoll.fd, req, 2024, 0);
// 	if(bytes <= 0)
// 		throw std::runtime_error("");
// 	return req;
// 	std::cout << bytes << std::endl;
// 	// try {
// 	// 	Request request;
// 	// 	request.parseRequest(req);
// 	// 	std::cout << request.getPathname() << std::endl;
// 	// 	std::cout << request.header("Host") << std::endl;

// 	// 	StatusCode status;
// 	// 	Response res;
// 	// 	if(!status.isFormed(request))
// 	// 	{
// 	// 		res.addRequestLine(status.getVersion(), std::to_string(status.getStatusCode()), status.getMsg());
// 	// 		res.addHeader("Server" , "webserv");
// 	// 		res.addBlankLine();
// 	// 		res.addBody(status.getMsg());
// 	// 	}
// 	// 	else
// 	// 	{
// 	// 		if (!status.isMatched(request.getPathname()))
// 	// 		{
				
// 	// 		}

// 	// 		res.addRequestLine(status.getVersion(), std::to_string(status.getStatusCode()), status.getMsg());
// 	// 		res.addHeader("Server" , "webserv");
// 	// 		res.addBlankLine();
// 	// 		res.addBody(status.getMsg());
// 	// 	}

// 	// 	// std::string ResponseBody;
// 	// 	// ResponseBody = readHtml("/Users/mzeroual/Desktop/webserv/src/Request/formulaire.html");
// 	// 	// write(clientFd, res.getResponse().c_str(), res.getResponse().length());
// 	// 	// close(clientFd);
// 	// 	return res.getResponse();
// 	// }
// 	// catch(...)
// 	// 	{
// 	// 		std::cerr << "error here " << std::endl;
// 	// 	}
// 	return "HTTP/1.1 200 OK \r\n\r\nhello";
// }

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


		std::cout << request << std::endl;
		req.parseRequest(request);
		
		


		StatusCode status;
		// status.
		Response res;
		
		// if(!status.isFormed(req) || !status.isMatched(req, servers.getAllServers()))
		if(!status.isMatched(req, servers.getAllServers()))
		{
			res.addRequestLine(status.getVersion(), std::to_string(status.getStatusCode()), status.getMsg());
			res.addHeader("Server" , "webserv");
			res.addBlankLine();
			res.addBody(status.getMsg());
		}
		else
		{
			// std::cout << "here --" << std::endl;
			// if ()
			// {
			// 	res.addRequestLine(status.getVersion(), std::to_string(status.getStatusCode()), status.getMsg());
			// 	res.addHeader("Server" , "webserv");
			// 	res.addBlankLine();
			// 	res.addBody(status.getMsg());
			// }

			res.addRequestLine(status.getVersion(), std::to_string(status.getStatusCode()), status.getMsg());
			res.addHeader("Server", "webserv");
			std::cout << req.getPathname() << std::endl << std::flush;
			std::cout << req.extention(req.getPathname()) << std::endl << std::flush;
			std::cout << res.getMimeType(req.extention(req.getPathname())) << std::endl << std::flush;
			std::string MIMEType = res.getMimeType(req.extention(req.getPathname()));
			if (!MIMEType.empty())
				res.addHeader("Content-Type", MIMEType);
			// res.addHeader("Content-Length" , req.header(std::to_string(status.getBody().length()));
			res.addBlankLine();
			res.addBody(status.getBody());
		}


		std::string respond;
		// std::cout << res.getResponse() << std::endl;
		write(working[i].fdpoll.fd, res.getResponse().c_str(), res.getResponse().size());
		// std::cout << "hiiiii" << std::endl;
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