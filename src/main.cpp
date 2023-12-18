#include "webserver.h"


unsigned int convertStringToBinary(String str)
{
	unsigned int res = 0;
	std::vector<String> strs = str.split('.');
	for (size_t i = 0; i < strs.size(); i++)
		res += (unsigned int)strtol(strs[i].c_str(), NULL, 10) << i * 8;
	return (res);
}


void	handleLogges(GeneralPattern& server)
{
	static int stdout_fd;
	static int stderr_fd;

	/**
	 * @attention possible fd leaks here.
	*/
	if (!stdout_fd && (stdout_fd = dup(STDOUT_FILENO)) == -1)
	{
		Logger::error(std::cerr , "dup() function failed.", "");
		return ;
	}
	if (!stderr_fd && (stderr_fd = dup(STDERR_FILENO)) == -1)
	{
		Logger::error(std::cerr , "dup() function failed.", "");
		return ;
	}
	if (dup2(stdout_fd, STDOUT_FILENO) == -1 || dup2(stderr_fd, STDERR_FILENO) == -1)
	{
		Logger::error(std::cerr , "dup2() function failed.", "");
		return ;
	}

	std::vector<Data> accessLog = server.getData("access_log");
	std::vector<Data> errorLog = server.getData("error_log");
	if (accessLog.empty() == false)
	{
		int file_fd = open(accessLog.at(0).getValue().c_str() , O_CREAT | O_APPEND | O_RDWR, 0777);
		if (file_fd == -1 || dup2(file_fd, STDOUT_FILENO) == -1 || close(file_fd) == -1)
		{
			close(file_fd);
			Logger::error(std::cerr , "failed to open access_log file : ", accessLog.at(0).getValue().c_str());
			return ;
		}
		close(file_fd);
	}
	if (errorLog.empty() == false)
	{
		int file_fd = open(errorLog.at(0).getValue().c_str() , O_CREAT | O_APPEND | O_RDWR, 0777);
		if (file_fd == -1 || dup2(file_fd, STDERR_FILENO) == -1)
		{
			close(file_fd);
			Logger::error(std::cerr , "failed to open error_log file : ", errorLog.at(0).getValue().c_str());
			return ;
		}
		close(file_fd);
	}
}

ResponseHeader	getErrorPage(std::vector<Data> errorPages, String errorNumber, String message)
{
	ResponseHeader responseHeader;
	for (size_t i = 0; i < errorPages.size(); i++)
	{
		std::vector<String> numbers = errorPages.at(i).getValue().split();
		for (size_t i = 0; i < numbers.size() - 1; i++)
			if (!numbers.at(i).compare(errorNumber))
				return (responseHeader.status("302	Found").location(*(numbers.end() - 1)));
	}
	responseHeader.status(errorNumber + " " + message).body(new String("<h1>" + errorNumber + " " + message + "</h1>"));
	return (responseHeader);
}

ResponseHeader	autoIndexing(GeneralPattern& loca, String	path)
{
	String content;
	String file;
	ResponseHeader responseHeader;

	std::vector<Data>	data = loca.getData("root");
	file = getRootPath(data.at(0).getValue(), path);
	if (loca.getData("alias").empty() == false)
	{
		data = loca.getData("alias");
		file = getAliasPath(data.at(0).getValue());
	}

	if (data.empty() == true)
		return (getErrorPage(loca.getData("error_page"), "404", "Not Found"));

	std::vector<Data> indexes = loca.getData("index");
	if (indexes.empty() == true)
		indexes.push_back(Data("index", "index.html"));

	std::vector<Data> autoIndex = loca.getData("autoindex");
	std::vector<Data> tryfiles = loca.getData("try_files");
	content = getFileContent(indexes.at(0).getValue().split(), file);

	if (!tryfiles.size() && content.empty() == true && loca.isExist(Data("autoindex", "off")))
		return (getErrorPage(loca.getData("error_page"), "403", "Forbidden"));

	if (content.empty() == true && loca.isExist(Data("autoindex", "on")))
	{
		String str = getDirectoryContent(file, path);
		if (str.empty() == false)
			return (responseHeader.body(new String(str)));
		return (getErrorPage(loca.getData("error_page"), "404", "Not Found"));
	}
	throw (std::exception());

}

ResponseHeader	to_do(GeneralPattern& targetInfo, String path)
{
	String file;
	ResponseHeader responseHeader;

	try {
		handleLogges(targetInfo);
		return (autoIndexing(targetInfo, path));
	}
	catch (std::exception&)
	{
	}
	std::vector<Data>	data = targetInfo.getData("root");
	file = getRootPath(data.at(0).getValue(), path);
	if (targetInfo.getData("alias").empty() == false)
	{
		data = targetInfo.getData("alias");
		file = getAliasPath(data.at(0).getValue());
	}
	if (data.empty() == true)
		return (getErrorPage(targetInfo.getData("error_page"), "404", "Not Found"));

	std::vector<Data> tryfiles = targetInfo.getData("try_files");
	if (tryfiles.empty() == false)
	{
		std::vector<String> files2try = tryfiles.at(0).getValue().split();
		file = tryFiles(files2try, file);
		if (file.empty() == false)
			responseHeader.fileName(file);
		else
			responseHeader.status("301 Moved Permanently").location(*(files2try.end() - 1));
		return (responseHeader);
	}

	std::vector<Data> indexes = targetInfo.getData("index");
	if (indexes.empty() == true)
		indexes.push_back(Data("index", "index.html"));
	for (size_t i = 0; i < indexes.size(); i++)
	{
		file = getFileContent(indexes.at(i).getValue().split(), file);
		if (file.empty() == false)
			return (responseHeader.fileName(file));
	}
	return (getErrorPage(targetInfo.getData("error_page"), "404", "Not Found"));
}

ResponseHeader	handler(ServerPattern& server, GeneralPattern &model, int readyFd)
{
	ResponseHeader	responseHeader;
	(void)readyFd;
	String method = model.getData("Method").begin()->getValue();
	String path(method.split()[1]);
	path.trim(" \t\r\n");
	String strHost = model.getData("Host").at(0).getValue();
	handleLogges(server);
	// Logger::success(std::cout, "REQUEST ==> ", method);
	std::vector<Data> roots = server.getData("root");
	String root;
	if (roots.empty() == false)
		root = roots.at(0).getValue();
	if (root.empty() == false && server.checkIsDirectory(root.append(path)) == 0)
	{
	responseHeader.fileName(root);
		return (responseHeader);
	}
	if (path.back() != '/')
		return (responseHeader.status("301 Moved Permanently").location(path + "/"));
	path.rightTrim("/");
	LocationPattern	loca = ServerPattern::getLocationByPath(server.getLocation(), path);
	std::vector<Data> returns = loca.getData("return");
	if (returns.empty() == false)
	{
		std::vector<String> values = returns.at(0).getValue().split();
		/**
		 * @attention status should be dynamic.
		*/
		responseHeader.status(values.at(0) + " Moved Permanently");
		if (values.size() == 2)
			responseHeader.location(values.at(1));
		return (responseHeader);
	}
	GeneralPattern target;
	if (loca.getPath().empty() == false)
		target = loca;
	else
		target = server;
	try
	{
		if (target.isExist(Data("autoindex", "on")))
			return (autoIndexing(target, path));
	}
	catch(...) {}
	return (to_do(target, loca.getPath()));
}


bool	requestHandler(const std::vector<int>& port, Server& server, ServerData& serv, int readyFd)
{
	struct sockaddr_in clientSocket;
	if (readyFd < 0)
		return (true);
	if (find(port.begin(), port.end(), readyFd) != port.end())
	{
		int newClient = server.accept(readyFd, (struct sockaddr *)&clientSocket);
		if (newClient < 0)
			return (false);
		server.fds.push_fd(newClient, clientSocket);
	}
	else
	{
		String header = server.recieve(readyFd);
		if (header.empty() == true)
			return (true);
		std::cout << header << std::endl;
		GeneralPattern model(Parser::parseHeader(header));
		ResponseHeader response;
		try
		{
			String strHost = model.getData("Host").at(0).getValue();
			struct sockaddr_in	clientInfo_in = server.fds.getClientInfo(readyFd);
			// struct sockaddr*	clientInfo = (struct sockaddr*)&clientInfo_in;
			std::cout << "================> " << clientInfo_in.sin_addr.s_addr << " IP ADDRESS : " << convertStringToBinary(String("10.11.3.7")) << " <======================" << std::endl;
			std::vector<ServerPattern> servModel = ServerData::getServer(serv, readyFd, strHost);
			if (servModel.empty() == true)
				servModel = serv.getAllServers();

			ServerPattern servPattern = servModel.at(0);
			if (servModel.empty())
				response = getErrorPage(servPattern.getData("error_page"), "400", "Bad Request");

			response = handler(servPattern, model, readyFd);
			String filename = response.getFileName();
			if (filename.empty() == false)
			{
				String* str = getContentFile(filename);
				if (!str)
					throw (std::exception());
				std::vector<Data> accept = model.getData("Accept");
				if (accept.empty() == false)
				{
					std::ostringstream oss;
					oss << str->size();
					response.contentLength(oss.str());
					if (accept.at(0).getValue().split(':').at(0).contains("image") == true)
						response.contentType("*/*");
					response.connection("close");
				}
				response.body(str);
			}
		}
		catch(const std::exception& e)
		{
			Logger::error(std::cerr, "catch exception in requestHandler function : ", e.what());
			response.status("500 Internal Server Error").body(new String("500 Internal Server Error"));
		}
		String *resStr = response.toString();
		static int nlog;
		std::cout << ++nlog << " ";
		Logger::success(std::cout, "Response ==> ", resStr->substr(0, resStr->find('\r')));
		// ssize_t resStrlen = (ssize_t)resStr->length();
		// ssize_t sender = server.send(readyFd, *resStr);
		server.send(readyFd, *resStr);
		// if (sender == resStrlen)
		// {
		close(readyFd);
		server.fds.erase_fd(readyFd);
		// }
		delete resStr;
	}
	return (true);
}

void	runServerByPoll(ServerData& serv, Server& server, std::vector<int> port)
{
	while (true)
	{
		Poll	tmpPoll(server.fds);
		int pollReturn = tmpPoll.waitingRequest();
		if (pollReturn < 0)
			break ;
		if (pollReturn == 0)
		{
			// for(size_t i = 0; i < server.fds.fdsSize(); i++)
			// {
			// 	close(server.fds.fds[i].fd);
			// 	server.fds.erase_fd(server.fds.fds[i].fd);
			// }
			continue ;
		}
		for (int i = 0; i < (int)tmpPoll.fdsSize(); i++)
			requestHandler(port, server, serv, tmpPoll.getReadyFd(i));
	}
}

Server	createServer(ServerData& serv)
{
	Server server;
	std::vector<int> ports = openAllPorts(serv.getAllServers(), server);
	runServerByPoll(serv, server, ports);
	for (size_t i = 0; i < ports.size(); i++)
		close(ports.at(i));
	return (server);
}

void	start(Parser& parser)
{
	ServerData servers(parser.getServers());
	try
	{
		createServer(servers);
	}
	catch (std::exception& e)
	{
		Logger::error(std::cerr, "I can't found the exact server, Reason => ", e.what());
	}
}

int	main(int ac, char **av)
{
	if (ac < 2)
	{
		Logger::error(std::cerr, "Invalid argument", ".");
		return (1);
	}
	try
	{
		Parser parser(av[1]);
		Checker checker(parser.getServers());
		checker.fullCheck();
		start(parser);
	}
	catch (ParsingException& e)
	{
		Logger::error(std::cerr, e.what(), "afdsfasdf");
	}
	return (0);
}