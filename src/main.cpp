#include "Parser.hpp"
#include "Checker.hpp"
#include "Server.hpp"
#include <dirent.h>

String  getContentFile(String file)
{
    std::fstream new_file;
    
    new_file.open(file, std::ios::in);
    if (new_file.is_open() == false)
        return ("");
    String  sa;
    String  content;
    while (std::getline(new_file, sa))
        content.append(sa).append("\n");
    new_file.close();
    return content;
}

String	getRootPath(String	root, String path)
{
	String file(root);
	file.append(path).append((file.end() - 1)[0] == '/' ? "" : "/");
	return (file);
}

String	getAliasPath(String	aliasPath)
{
	aliasPath.rightTrim("/");
	return (aliasPath.append("/"));
}

String	tryFiles(const std::vector<String>& files, const String& path)
{
	String value;
	for (size_t i = 0; i < files.size() - 1; i++)
	{
		String tmp(path);
		tmp.append(files.at(i));
		value = readFile(tmp);
		if (value.length() != 0)
			return (value);
	}
	return (value);
}

String	getFileContent(const std::vector<String>& indexes, const String& path)
{
	String value;
	for (size_t i = 0; i < indexes.size(); i++)
	{
		String tmp(path);
		tmp.append(indexes.at(i));
		value = readFile(tmp);
		if (value.length() != 0)
			return (value);
	}
	return (value);
}

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
		body.append("<a href=\"").append(path)
			.append("/").append(dirp->d_name)
			.append("\">").append(dirp->d_name)
			.append("</a><br>");
	body.append("</pre><hr>");
	closedir(dir);
	return (body);
}

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
	std::vector<Data> autoIndex = loca.getData("autoindex");
	std::vector<Data> tryfiles = loca.getData("try_files");
	if (!tryfiles.size() 
		&& indexes.empty() == true && (autoIndex.empty() == true || (autoIndex.empty() == false && !autoIndex.at(0).getValue().compare("off"))))
		return (ERROR_403);
	if (autoIndex.empty() == false && !autoIndex.at(0).getValue().compare("on"))
		return (getDirectoryContent(file, path));
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

String	handler(ServerData& servers, std::vector<Data> header)
{
	GlobalModel model(header);
	std::vector<ServerModel>	servModel;
	String	content;

	servModel = getServer(servers, header);
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
		{
			content.append("Content-Length: ");
			content.append(std::to_string(str.size()));
			content.append("\r\nContent-Type: ");
			content.append(accept.at(0).getValue().split(',').at(0));
			content.append("\r\n");
		}
		content.append("\r\n");
		return (content.append(str));
	}
	path.rightTrim("/");
	content.append("\r\n");
	Location	loca = ServerModel::getLocationByPath(servModel.at(0).getLocation(), path);
	if (loca.getPath().empty() == false)
		return (content.append(to_do(loca)));
	return (content.append(ERROR_404));
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
			// std::cout << header << std::endl << std::endl;
			String content("HTTP/1.1 200 OK\r\n");
			content.append(handler(serv, Parser::parseHeader(header)));
			ssize_t sender = server.send(readyFd, content);
			if (sender == -1)
			{
			}
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