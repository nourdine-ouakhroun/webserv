#include "Server.hpp"

std::string readHtml(std::string file)
{
	std::ifstream	in;
	std::string		content;
	std::string		input;

	in.open(file);
	if (in.is_open()) {
		while (std::getline(in, input,'\n'))
		{
			content += input;
		}
	}
	else
		std::cout << "file html.html not open"<< std::endl;
	return (content);
}


int	errorExit(int status, std::string errorMsg)
{
	if (status < 0)
	{
		std::cerr << errorMsg << std::endl;
		exit (1);
	}
	return (status);
}

Server::Server()
{
	bzero(&servAddr ,sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	servFd = errorExit(socket(AF_INET, SOCK_STREAM, 0), "Error : socket!.");
	fcntl(servFd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	errorExit(bind(servFd, (SA *)&servAddr, sizeof(servAddr)), "Error : bind!.");
	errorExit(listen(servFd, 10), "Error : listen!.");

	// int countReq = 1 ;
	std::cout << "http://localhost:" << PORT << std::endl;
	while (1)
	{
		clientFd = accept(servFd, NULL, NULL);
		if (clientFd < 0)
			continue ;
		fcntl(clientFd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
		sleep(2);
		
		std::string request = readRequest(clientFd);

		Request req;
		req.parseRequest(request);
		StatusCode status;
		Response res;
		if(!status.isFormed(req))
		{
			res.addRequestLine(status.getVersion(), std::to_string(status.getStatusCode()), status.getMsg());
			res.addHeader("Server" , "webserv");
			res.addBlankLine();
			res.addBody(status.getMsg());
		}
		else
		{
			if (!status.isMatched(req.getPathname()))
			{
				
			}

			res.addRequestLine(status.getVersion(), std::to_string(status.getStatusCode()), status.getMsg());
			res.addHeader("Server" , "webserv");
			res.addBlankLine();
			res.addBody(status.getMsg());
		}
		std::string ResponseBody;
		ResponseBody = readHtml("/Users/mzeroual/Desktop/webserv/src/Request/formulaire.html");
		write(clientFd, res.getResponse().c_str(), res.getResponse().length());
		close(clientFd);

		

	}
}

Server::~Server()
{
	close(servFd);
	// close(newfd);
}
