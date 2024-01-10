#include "webserver.h"


unsigned int convertStringToBinary(String str)
{
	unsigned int res = 0;
	std::vector<String> strs = str.split('.');
	for (size_t i = 0; i < strs.size(); i++)
		res += (unsigned int)strtol(strs[i].c_str(), NULL, 10) << i * 8;
	return (res);
}


ResponseHeader	to_do(GeneralPattern& targetInfo, String path)
{
	String file;
	ResponseHeader responseHeader;

	handleLogges(targetInfo);

	std::vector<Data>	roots = targetInfo.getData("root");
	if (roots.empty() == true)
		return (errorPage(targetInfo.getData("error_page"), "404", "Not Found"));
	
	{
		/**
		 * @name root and alias directives.
		 * @details In Nginx, when you have both the root directive and the alias directive within the same location block, the alias directive takes precedence over the root directive.
		 * 	1. alias
		 * 	2. root
		*/
		file = getRootPath(roots.at(0).getValue(), path);
		if (targetInfo.getData("alias").empty() == false)
			file = getAliasPath(targetInfo.getData("alias").at(0).getValue());
	}

	{
		/**
		 * @brief the priority for serving files is determined by the directives used in the configuration.
		 * 	1. try_files
		 * 	2. index
		 * 	3. auto_index on
		*/

		std::vector<Data> tryfiles = targetInfo.getData("try_files");
		if (tryfiles.empty() == false)
		{
			std::vector<String> files2try = tryfiles.at(0).getValue().split();
			file = tryFiles(files2try, file);
			if (file.empty() == false)
				responseHeader.fileName(file);
			else
				responseHeader.status("301 Moved Permanently").location(files2try.back());
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

		if (targetInfo.isExist(Data("autoindex", "on")))
			return (autoIndexing(targetInfo, file, path));
	}

	return (errorPage(targetInfo.getData("error_page"), "404", "Not Found"));
}

ResponseHeader	handler(ServerPattern& server, GeneralPattern &model, int readyFd)
{
	ResponseHeader	responseHeader;
	(void)readyFd;
	String method = model.getData("Method").front().getValue();
	String path(method.split()[1]);
	path.trim(" \t\r\n");
	String strHost = model.getData("Host").front().getValue();
	handleLogges(server);

	// std::cout << "model.getData(\"Host\")[0].getValue() : " << model.getData("Host")[0].getValue() << std::endl;

	std::vector<Data> roots = server.getData("root");
	String root;
	if (roots.empty() == false)
		root = roots.front().getValue();

	{
		// check is url is a file.
		if (root.empty() == false && server.checkIsDirectory(root.append(path)) == 0)
			return (responseHeader.fileName(root));
	}

	{
		// redirect the path that doesn't containt '/' in the end.
		if (path.back() != '/')
			return (responseHeader.status("301 Moved Permanently").location(path + "/"));
	}

	path.rightTrim("/");
	LocationPattern	loca = ServerPattern::getLocationByPath(server.getLocation(), path);

	{
		// check is there's a return directive in this location.
		std::vector<Data> returns = loca.getData("return");
		if (returns.empty() == false)
			return (returnDirective(returns, responseHeader));
	}


	GeneralPattern target;
	try
	{
		target = (!loca.getPath().empty()) ? dynamic_cast<GeneralPattern&>(loca) : dynamic_cast<GeneralPattern&>(server);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}


	// if (target.isExist(Data("autoindex", "on")))
	// 	return (autoIndexing(target, root, path));
	
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

			std::vector<ServerPattern> servModel = ServerData::getServer(serv, readyFd, strHost);
			if (servModel.empty() == true)
				servModel = serv.getAllServers();

			ServerPattern servPattern = servModel.at(0);
			if (servModel.empty())
				response = errorPage(servPattern.getData("error_page"), "400", "Bad Request");

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