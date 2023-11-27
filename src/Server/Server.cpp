

#include "server.hpp"



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

	int countReq = 1 ;
		std::cout << "http://localhost:" << PORT << std::endl;
	while (1) {
		clientFd = accept(servFd, NULL, NULL);
		// std::cout << clientFd << std::endl;
		if (clientFd < 0)
			continue ;
		fcntl(clientFd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
		sleep(2);
		// std::cout << clientFd << std::endl;
		

		std::string request = readRequest(clientFd);
		// std::cout << "-----------------------------------------------------" << std::endl;
		// std::cout << request << std::endl;
		// std::cout << "countReq : " << countReq++ << std::endl;
		// std::cout << "-----------------------------------------------------" << std::endl;
		Request req(request);
		RequestLine reqLine = req.parse(request);

		std::cout << reqLine.getMethode() << std::endl;
		std::cout << reqLine.getQuery() << std::endl;
		std::cout << reqLine.getHttpVersion() << std::endl;
		// std::string website;
		// if (req.getRequestLine()[1].size() == 1)
		// 	 website = readHtml("/Users/mzeroual/Desktop/cursus/webserv/src/Server/templatemo_562_space_dynamic/index.html");
		// else
		// 	 website = readHtml("/Users/mzeroual/Desktop/cursus/webserv/src/Server/templatemo_562_space_dynamic/"+req.getRequestLine()[1]);
		// // std::cout << website ;
		// size_t pos;
		// std::string extention;
		// if ((pos = req.getRequestLine()[1].rfind(".")) != std::string::npos)
		// {
		// 	extention = req.getRequestLine()[1].substr(pos + 1, req.getRequestLine()[1].length());
		// 	// std::cout << "Extention = " << req.getMimeType(extention) << std::endl;
		// }
		std::string	response = "HTTPv1_0 200 OK\r\nContent-Type: image/png\r\n\r\nhttps://profile.intra.42.fr/assets/42_logo_black-684989d43d629b3c0ff6fd7e1157ee04db9bb7a73fba8ec4e01543d650a1c607.png";

		write(clientFd, response.c_str(), strlen(response.c_str()));
		close(clientFd);
	}
}

Server::~Server()
{
	close(servFd);
	// close(newfd);
}
