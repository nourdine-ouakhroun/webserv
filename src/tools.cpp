#include "Server.hpp"
#include "Logger.hpp"
#include "Directives.hpp"
#include "Socket.hpp"

String	readFile(const String& path)
{
	String content;
	char	buffer[2];

	int fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
		return (content);
	while (1)
	{
		memset(buffer, 0, 2);
		ssize_t reader = read(fd, buffer, 1);
		content.append(buffer);
		if (reader != 1)
			break ;
	}
	close(fd);
	return (content);
}

unsigned short	getPort(String	value)
{
	unsigned short port = 80;
	String::size_type pos = value.find_last_of(':');
	if (pos != String::npos)
		port = (unsigned short)strtol(value.substr(pos + 1).c_str(), NULL, 10);
	return port;
}

// vector<int>	openAllPorts(const vector<ServerPattern>& serversInfo, Server& server)
// {
// 	vector<int> ports;
// 	vector<String> tmpInfo;
// 	int newSocket;
// 	for (size_t i = 0; i < serversInfo.size(); i++)
// 	{
// 		vector<Data> data = serversInfo[i].getData("listen");
// 		for (size_t i = 0; i < data.size(); i++)
// 		{
// 			if (find(tmpInfo.begin(), tmpInfo.end(), data[i].getValue()) != tmpInfo.end())
// 				continue ;
// 			tmpInfo.push_back(data[i].getValue());
// 			vector<String> listen = data[i].getValue().split(':');
// 			unsigned short port = (unsigned short)strtol(listen[1].c_str(), NULL, 10);
// 			newSocket = server.createNewSocket(listen[0], port);
// 			if (newSocket == -1)
// 				continue ;
// 			Logger::info(cout, "Listen to port : ", port);
// 			ports.push_back(newSocket);
// 		}
// 	}
// 	tmpInfo.clear();
// 	if (ports.empty() == true)
// 		throw (ServerException("Invalid Ports."));
// 	return (ports);
// }

ResponseHeader servingFileContent(GeneralPattern &targetInfo, String path)
{
	String root;
	String fileName;
	Directives directive(targetInfo, path);
	ResponseHeader responseHeader;

	root = directive.getRootPath();

	if (!targetInfo.getData("try_files").empty())
		return (directive.tryFiles());

	fileName = directive.indexing();
	if (!fileName.empty())
		return (responseHeader.fileName(fileName));

	if (!targetInfo.getData("return").empty())
		return (directive.returnDirective());

	if (targetInfo.isExist(Data("autoindex", "on")))
		return (directive.autoIndexing());

	return (directive.errorPage("404", "Not Found"));
}

ResponseHeader to_do(GeneralPattern &targetInfo, String path, String &model)
{
	String root;
	String fileName;
	Directives directive(targetInfo, path);
	ResponseHeader responseHeader;

	if (targetInfo.getAllData().empty())
		return (directive.errorPage("404", "Not Found"));
	// directive.handleLogges();

	vector<Data> roots = targetInfo.getData("root");
	if (roots.empty() == false)
		root = roots.front().getValue();

	if (ServerPattern::checkIsDirectory(root + model) == 0) // check is url is a file.
		return (responseHeader.fileName(root + model));

	if (model.back() != '/') // redirect the path that doesn't containt '/' in the end.
		return (responseHeader.status("301 Moved Permanently").location(model + "/"));

	if (model.size() != 1)
		model.rightTrim("/");

	if (path != model)
		return (directive.errorPage("404", "Not Found"));

	// if (ServerPattern::checkIsDirectory(root) == 0) // check is url is a file.
	// 	return (responseHeader.fileName(root));

	return (servingFileContent(targetInfo, path));
}

ResponseHeader handler(ServerPattern &server, GeneralPattern &model)
{
	ResponseHeader responseHeader;
	String method = model.getData("Method").front().getValue();
	String path(method.split()[1]);
	path.trim(" \t\r\n");

	vector<String> pathss;
	ServerPattern::getAllLocationPath(server.getLocations(), pathss);
	vector<String> newPaths;
	String test(path);
	test.rightTrim("/");
	for (size_t i = 0; i < pathss.size(); i++)
	{
		if (!strncmp(pathss[i].c_str(), test.c_str(), pathss[i].size()) && pathss[i].size() == test.size())
		{
			newPaths.clear();
			newPaths.push_back(pathss[i]);
			break;
		}
		if (!strncmp(pathss[i].c_str(), test.c_str(), pathss[i].size()))
			newPaths.push_back(pathss[i]);
	}

	vector<String>::iterator it = max_element(newPaths.begin(), newPaths.end());
	if (it == newPaths.end())
		return (responseHeader);
	cout << "Larget string : " << *it << endl;
	LocationPattern loca = ServerPattern::getLocationByPath(server.getLocations(), *it);
	GeneralPattern target;

	try
	{
		target = dynamic_cast<GeneralPattern &>(loca);
		if (loca.getPath().empty() && path.equal("/") && path.size() == 1)
			target = dynamic_cast<GeneralPattern &>(server);
	}
	catch (const exception &e)
	{
		cerr << e.what() << '\n';
	}
	return (to_do(target, loca.getPath(), path));
}

void getResponse(ServerData &servers, Socket &socket)
{
	cout << socket.getHeader() << endl;
	GeneralPattern header(Parser::parseHeader(socket.getHeader()));
	vector<ServerPattern> server = ServerData::getServer(servers, socket.ipAndPort, header.getData("Host").front().getValue());
	ResponseHeader response = handler(server.front(), header);
	String filename = response.getFileName();
	if (filename.empty() == false)
	{
		String *str = getContentFile(filename);
		if (!str)
			throw(exception());
		vector<Data> accept = header.getData("Accept");
		if (accept.empty() == false)
		{
			ostringstream oss;
			oss << str->size();
			response.contentLength(oss.str());
			if (accept.at(0).getValue().split(':').at(0).contains("image") == true)
				response.contentType("*/*");
			response.connection("close");
		}
		response.body(str);
	}
	String *str = response.toString();
	socket.respond = *str;
	delete str;
}
