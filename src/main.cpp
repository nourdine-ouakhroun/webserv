#include "webserver.h"


template <typename T>
void	handleLoggges(const T& server)
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
	responseHeader.status(errorNumber + " " + message).body("<h1>" + errorNumber + " " + message + "</h1>");
	return (responseHeader);
}

template <typename T>
ResponseHeader	autoIndexing(T& loca, String	path)
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

	if (!tryfiles.size() && content.empty() == true && (autoIndex.empty() == false && !autoIndex.at(0).getValue().compare("off")))
		return (getErrorPage(loca.getData("error_page"), "403", "Forbidden"));

	if (content.empty() == true && autoIndex.empty() == false && !autoIndex.at(0).getValue().compare("on"))
		return (responseHeader.body(getDirectoryContent(file, path)));
	throw (std::exception());
}

template <typename T>
ResponseHeader	to_do(const T& loca, String path)
{
	String content;
	String file;
	ResponseHeader responseHeader;

	try {
		handleLoggges(loca);
		return (autoIndexing(loca, path));
	}
	catch (std::exception&){

	}
	Logger::success(std::cout, "hello world", " mehdi salim");
	std::vector<Data>	data = loca.getData("root");
	file = getRootPath(data.at(0).getValue(), path);
	if (loca.getData("alias").empty() == false)
	{
		data = loca.getData("alias");
		file = getAliasPath(data.at(0).getValue());
	}
	if (data.empty() == true)
		return (getErrorPage(loca.getData("error_page"), "404", "Not Found"));

	std::vector<Data> tryfiles = loca.getData("try_files");
	if (tryfiles.empty() == false)
	{
		std::vector<String> files2try = tryfiles.at(0).getValue().split();
		content = tryFiles(files2try, file);
		if (content.empty() == false)
			responseHeader.body(content);
		else
			responseHeader.status("301 Moved Permanently").location(*(files2try.end() - 1));
		return (responseHeader);
	}

	std::vector<Data> indexes = loca.getData("index");
	if (indexes.empty() == true)
		indexes.push_back(Data("index", "index.html"));
	for (size_t i = 0; i < indexes.size(); i++)
	{
		content = getFileContent(indexes.at(i).getValue().split(), file);
		if (content.empty() == false)
			return (responseHeader.body(content));
	}
	return (getErrorPage(loca.getData("error_page"), "404", "Not Found"));
}

ResponseHeader	handler(ServerData& servers, GlobalModel &model)
{
	std::vector<ServerModel>	servModel;
	ResponseHeader	responseHeader;

	servModel = getServer(servers, model.getAllData());
	if (servModel.empty() == true)
		servModel = servers.getAllServers();
	String method = model.getData("Method").begin()->getValue();
	String path(method.split()[1]);
	path.trim(" \t\r\n");
	ServerModel server = servModel.at(0);
	handleLoggges(server);
	Logger::success(std::cout, "REQUEST ==> ", method);
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
			std::ostringstream oss;
			oss << str.size();
			responseHeader.contentLength(oss.str());
		}
		responseHeader.body(str);
		return (responseHeader);
	}
	if (*(path.end() - 1) != '/')
		return (responseHeader.status("301 Moved Permanently").location(path + "/"));
	path.rightTrim("/");
	Location	loca = ServerModel::getLocationByPath(servModel.at(0).getLocation(), path);
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
	if (loca.getPath().empty() == false)
		return (to_do(loca, loca.getPath()));
	if (path.empty() == true || (path.size() == 1 && path.at(0) == '/'))
		return (to_do(servModel.at(0), ""));
	try { return (autoIndexing(servModel.at(0), path)); }
	catch (std::exception&) {}

	return (getErrorPage(servModel.at(0).getData("error_page"), "404", "Not Found"));
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
			GlobalModel model(Parser::parseHeader(header));
			ResponseHeader response;
			try
			{
				response = handler(serv, model);
			}
			catch(const std::exception& e)
			{
				Logger::error(std::cerr, "catch exception in requestHandler function : ", e.what());
				response.status("500 Internal Server Error");
			}
			String resStr = response.toString();
			Logger::success(std::cout, "RESPONSE ==> ", resStr.substr(0, resStr.find('\r')));
			server.send(readyFd, resStr);

			String method(model.getData("Method").begin()->getValue().split()[0]);
			if (!method.compare("GET"))
			{
				close(readyFd);
				server.fds.erase_fd(readyFd);
			}
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