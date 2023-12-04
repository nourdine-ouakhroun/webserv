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

// void checkRequest(std::string request)
// {
// 	std::string					line;
// 	std::vector<std::string>	content;
// 	size_t						pos = 0;

// 	while ((pos = request.find("\r\n")) != std::string::npos)
// 	{
// 		if (request == "\r\n")
// 			break ;
// 		line = request.substr(0, pos);
// 		request = request.substr(pos + 2, request.length());
// 		// line += '\n';
// 		content.push_back(line);

// 	}
// 	// line = line.substr(0, line.length() - 1);
// 	size_t i = 0;
// 	while (i < content.size())
// 	{
// 		std::cout << content[i];
// 		i++;
// 	}
// }

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
		// size_t pos = request.find("\r\n");
		Request req;
		req.parseRequest(request);
		// std::cout << req.header("boundary") << std::endl;
		// std::cout << get.getMethod() << std::endl;
		// std::cout << get.getUrl() << std::endl;
		// std::cout << get.getVersion() << std::endl;
		// std::cout << get.getPathname() << std::endl;
		// std::cout << get.getQuery() << std::endl;

		// GetMethod get(request);
		
		// get.parseRequestLine(request.substr(0, pos));
		// std::cout << "#" << request << "#" << std::endl;
		// Response	res;
		// Request		req(request);
		// req.parseRequest(request);
		
		// RequestLine		reqLine = req.getRequestLine();
		// RequestHeader	reqHeader = req.getRequestHeader();
		// RequestBody		reqBody = req.getRequestBody();

		// std::cout << reqBody.getQuery() << std::endl;
		// std::cout << req.getRequestBody() << std::endl;
		// std::cout << reqHeader.header("") << std::endl;

		// res.makeResponse();

		std::string ResponsBody;
		// if (reqLine.getMethode() == "GET")
		// {
			// if (reqLine.pathname().length() == 1)
				ResponsBody = readHtml("/Users/mzeroual/Desktop/webserv/src/Request/formulaire.html");
			// else
				// ResponsBody = readHtml("/Users/mzeroual/Desktop/cursus/webserv/src/Request/" + reqLine.pathname());
		// }
		// 								// 		"HTTP/1.1 200 OK\n"
		// 								// "Date: Thu, 19 Feb 2009 12:27:04 GMT\n"
		// 								// "Server: Apache/2.2.3\n"
		// 								// "Last-Modified: Wed, 18 Jun 2003 16:05:58 GMT\n"
		// 								// "ETag: \"56d-9989200-1132c580\"\n"
		// 								// "Content-Type: text/html\n"
		// 								// "Content-Length: 15\n"
		// 								// "Accept-Ranges: bytes\n"
		// 								// "Connection: close\n"
		// 										// ss << 200;
		std::string response = "HTTP/1.1 200 OK\r\n\r\n" + ResponsBody;
		// std::stringstream ss;
		// ss << ResponsBody.length();
		// res.setResponseLine(reqLine.getHttpVersion(), "200", "OK");
		// res.setHeader("Connection", reqHeader.header("Connection"));
		// res.setHeader("Server", "Dokoko/v_1_0");
		// res.setHeader("Content-Length", ss.str());
		// res.setEndHeader();
		// res.setBody(ResponsBody);
		// response = "HTTP/1.0 200 ok\r\n\r\nhello";
		// write(clientFd, res.getResponse().c_str(), res.getResponse().length());
		write(clientFd, response.c_str(), response.length());
		close(clientFd);
	}
}

Server::~Server()
{
	close(servFd);
	// close(newfd);
}
