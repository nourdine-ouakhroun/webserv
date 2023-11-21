#include "Parser.hpp"
#include "Checker.hpp"
#include "Server.hpp"
#include <dirent.h>

String	checkIsFile(const std::vector<Location>& locations, String filePath);


String	getDirectoryContent(const String& dirname, String path)
{
	DIR	*dir = opendir(dirname.c_str());
	if (!dir)
		return "";
	String body("<h1>Index of ");
	body.append(path).append("</h1><hr><pre>");
	struct dirent *dirp;
	path.trim("/");
	while ((dirp = readdir(dir)) != NULL)
	{
		/**
		 * @brief body end with this form.
		 *		<h1>Index of /test</h1><hr><pre><a href="/test/.">.</a><br>
		 */
		body.append("<a href=\"").append(path).append("/").append(dirp->d_name).append("\">");
		body.append(dirp->d_name).append("</a><br>");
		// std::cout << dirp->d_name << std::endl;
		// std::cout << body << std::endl;
	}
	body.append("</pre><hr>");
	closedir(dir);
	return (body);
}

String	to_do(const Location& loca)
{
	String	path(loca.getPath());
	std::vector<Data>	data = loca.getData("root");
	if (data.empty() == true)
		return (ERROR_404);
	String file(data.at(0).getValue());
	file.append(path).append((file.end() - 1)[0] == '/' ? "" : "/"); // -> check if the path ended with '/'
	std::vector<Data> indexes = loca.getData("index");
	std::vector<Data> autoIndex = loca.getData("autoindex");
	if (indexes.empty() == true && (autoIndex.empty() == true || (autoIndex.empty() == false && !autoIndex.at(0).getValue().compare("off"))))
		return (ERROR_403);
	if (autoIndex.empty() == false && !autoIndex.at(0).getValue().compare("on"))
		return (getDirectoryContent(file, path));
	std::vector<String> indexs = String(indexes.at(0).getValue()).split();
	for (size_t i = 0; i < indexs.size(); i++)
	{
		String tmp(file);
		tmp.append(indexs.at(i));
		value = readFile(tmp);
		if (value.length() != 0)
			return (value);
	}
	return (ERROR_404);
}


String	handler(ServerData& servers, std::vector<Data> header)
{
	GlobalModel model(header);
	std::vector<ServerModel>	servModel;
	String	content;

	servModel = getServer(servers, header);
	if (servModel.empty() == true)
		servModel = servers.getAllServers();
	String path(String(model.getData("Method").begin()->getValue()).split()[1]);
	path.trim(" \t\r\n");
	ServerModel server = servModel.at(0);
	std::vector<Data> roots = server.getData("root");
	String root;
	if (roots.empty() == false)
		root = roots.at(0).getValue();
	Logger::success(std::cout, "path : ", path);
	if (root.empty() == false && server.checkIsDirectory(root.append(path)) == 0)
		return (content.append(readFile(root)));
	content = checkIsFile(servModel.at(0).getLocation(), path);
	if (content.empty() == false)
		return (content);
	path.rightTrim("/");
	Location	loca = ServerModel::getLocationByPath(servModel.at(0).getLocation(), path);
	content = ERROR_404;
	if (loca.getPath().empty() == false)
		to_do(loca, content);
	return (content);
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
			// std::cout << header << std::endl;
			String content("HTTP/1.1 200 OK\r\n\r\n");
			content.append(handler(serv, Parser::parseHeader(header)));
			ssize_t sender = server.send(readyFd, content);
			if (sender == -1)
				Logger::error(std::cerr, "Send Failed.", "");
			// std::cout << "Sender : " << sender << std::endl;
			close(readyFd);
			server.fds.erase_fd(readyFd);
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
			if (requestHandler(port, server, serv, tmpPoll.getReadyFd(i)) == false)
				break ;
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
		// servers.displayServers();
		createServer(servers);
	}
	catch (std::exception& e)
	{
		Logger::error(std::cerr, "I can't found the exact server, Reason => ", e.what());
	}
}


/**
 * @brief	main function.
 */
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

/**
 * String str("the configuration file");
 * str.append(av[1]);
 * Logger::success(std::cout, str, " syntax is ok.");
 * Logger::success(std::cout, str, " test is successfuli.");
*/

// ACCESS_LOG
// {
		// exit(0);
	// String access_log = servModel.at(0).getData("access_log").at(0).getValue();
	// if (access_log.contains("main") == true)
	// {
	// 	std::ofstream accessLogFile(access_log.split()[0].trim(" \t\n\r"));
	// 	Logger::info(accessLogFile, "Hello World", " Test 1");
	// }
	// std::vector<Data> hosts = model.getData("Method");
// }
