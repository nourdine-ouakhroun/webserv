#include "webserver.h"

String	to_do(const Location& loca)
{
	String content;
	String	path(loca.getPath());
	String file;
	std::vector<Data>	data = loca.getData("root");
	file = getRootPath(data.at(0).getValue(), path);
	if (loca.getData("alias").empty() == false)
	{
		data = loca.getData("alias");
		file = getAliasPath(data.at(0).getValue());
	}
	if (data.empty() == true)
		return (ERROR_404);
	std::vector<Data> indexes = loca.getData("index");
	if (indexes.empty() == true)
		indexes.push_back(Data("index", "index.html"));
	std::vector<Data> autoIndex = loca.getData("autoindex");
	std::vector<Data> tryfiles = loca.getData("try_files");
	content = getFileContent(indexes.at(0).getValue().split(), file);
	if (!tryfiles.size() 
		&& content.empty() == true && (autoIndex.empty() == true || (autoIndex.empty() == false && !autoIndex.at(0).getValue().compare("off"))))
		return (ERROR_403);
	if (content.empty() == true && autoIndex.empty() == false && !autoIndex.at(0).getValue().compare("on"))
		return (getDirectoryContent(file, path));
	content.clear();
	if (tryfiles.empty() == false)
	{
		content = tryFiles(tryfiles.at(0).getValue().split(), file);
		if (content.empty() == false)
			return (content);
	}
	for (size_t i = 0; i < indexes.size(); i++)
	{
		content = getFileContent(indexes.at(i).getValue().split(), file);
		if (content.empty() == false)
			return (content);
	}
	return (ERROR_404);
}

ResponseHeader	handler(ServerData& servers, GlobalModel &model)
{
	std::vector<ServerModel>	servModel;
	ResponseHeader	responseHeader;

	servModel = getServer(servers, model.getAllData());
	if (servModel.empty() == true)
		servModel = servers.getAllServers();
	String path(model.getData("Method").begin()->getValue().split()[1]);
	path.trim(" \t\r\n");
	ServerModel server = servModel.at(0);
	std::vector<Data> roots = server.getData("root");
	String root;
	if (roots.empty() == false)
		root = roots.at(0).getValue();
	if (root.empty() == false && server.checkIsDirectory(root.append(path)) == 0)
	{
		String str = getContentFile(root);
		std::vector<Data> accept = model.getData("Accept");
		if (accept.empty() == false)
			responseHeader.contentLength(std::to_string(str.size())).contentType(accept.at(0).getValue());
		responseHeader.body(str);
		return (responseHeader);
	}
	// if (*(path.end() - 1) != '/')
	// {
	// 	responseHeader.status("301 Moved Permanently").location("127.0.0.1:8080" + path + "/");
	// 	return (responseHeader);
	// }
	path.rightTrim("/");
	Location	loca = ServerModel::getLocationByPath(servModel.at(0).getLocation(), path);
	if (loca.getPath().empty() == false)
		return (responseHeader.body(to_do(loca)));
	responseHeader.status("404 Not Found").body(ERROR_404);
	return (responseHeader);
}

bool	requestHandler(const std::vector<int>& port, Server& server, ServerData& serv, int readyFd)
{
	if (readyFd > -1)
	{
		if (find(port.begin(), port.end(), readyFd) != port.end())
		{
			int newClient = server.accept(readyFd);
			if (newClient < 0)
				return (false);
			server.fds.push_fd(newClient);
		}
		else
		{
			String header = server.recieve(readyFd);
			if (header.empty() == true)
				return (true);
			std::cout << header << std::endl;
			GlobalModel model(Parser::parseHeader(header));
			ResponseHeader response = handler(serv, model);
			// ssize_t sender = server.send(readyFd, content);
			server.send(readyFd, response.toString());
			std::vector<Data> connection = model.getData("Connection");
			if (connection.empty() == false && connection.at(0).getValue().contains("keep-alive") == false)
			{
			}
			close(readyFd);
			server.fds.erase_fd(readyFd);

			// if (sender == -1)
			// {
			// }
		}
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
			continue ;
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
		Logger::error(std::cerr, e.what(), "");
	}
	return (0);
}