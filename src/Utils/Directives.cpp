#include "Directives.hpp"

Directives::Directives(const GeneralPattern &_data, const String& _path = "") : data(_data), path(_path)
{
}

Directives::~Directives( void )
{
}

void	Directives::handleLogges( void )
{
	static int stdout_fd;
	static int stderr_fd;

	vector<Data> accessLog = data.getData("access_log");
	vector<Data> errorLog = data.getData("error_log");
	if (accessLog.empty() && errorLog.empty())
		return ;
	/**
	 * @attention check fd leaks here.
	*/
	if (!stdout_fd && (stdout_fd = dup(STDOUT_FILENO)) == -1)
	{
		Logger::error(cerr , "dup() function failed.", "");
		return ;
	}
	if (!stderr_fd && (stderr_fd = dup(STDERR_FILENO)) == -1)
	{
		Logger::error(cerr , "dup() function failed.", "");
		return ;
	}
	if (dup2(stdout_fd, STDOUT_FILENO) == -1 || dup2(stderr_fd, STDERR_FILENO) == -1)
	{
		Logger::error(cerr , "dup2() function failed.", "");
		return ;
	}

	if (accessLog.empty() == false)
	{
		int file_fd = open(accessLog.at(0).getValue().c_str() , O_CREAT | O_APPEND | O_RDWR, 0777);
		if (file_fd == -1 || dup2(file_fd, STDOUT_FILENO) == -1 || close(file_fd) == -1)
		{
			close(file_fd);
			Logger::error(cerr , "failed to open access_log file : ", accessLog.at(0).getValue().c_str());
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
			Logger::error(cerr , "failed to open error_log file : ", errorLog.at(0).getValue().c_str());
			return ;
		}
		close(file_fd);
	}
}

ResponseHeader	Directives::errorPage(String errorNumber, String message)
{
	vector<Data> errorPages = data.getData("error_page");
	ResponseHeader responseHeader;
	for (size_t i = 0; i < errorPages.size(); i++)
	{
		vector<String> numbers = errorPages.at(i).getValue().split();
		for (size_t i = 0; i < numbers.size() - 1; i++)
			if (!numbers.at(i).compare(errorNumber))
				return (responseHeader.status(getStatusCode()["302"]).location(numbers.back()));
	}
	responseHeader.status(errorNumber + " " + message).body(new String("<h1>" + errorNumber + " " + message + "</h1>"));
	return (responseHeader);
}

ResponseHeader	Directives::autoIndexing( void )
{
	String content;
	String file;
	ResponseHeader responseHeader;

	String str = getDirectoryContent(getRootPath(), path);
	if (str.empty())
		return (errorPage("404", "Not Found"));
	return (responseHeader.body(new String(str)));
}

ResponseHeader		Directives::tryFiles( void )
{
	String value;
	vector<String> files = data.getData("try_files").front().getValue().split();
	for (size_t i = 0; i < files.size() - 1; i++)
	{
		String tmp(files[i]);
		String::size_type pos = tmp.find("$uri");
		if (pos != String::npos)
			tmp.replace(pos, 4, path);
		if (checkFile(tmp) == true)
			return (ResponseHeader().fileName(tmp));
	}
	return (ResponseHeader().status("301 Moved Permanently").location(files.back()));
}

String		Directives::indexing( void )
{
	String value;
	vector<Data> indexes = data.getData("index");
	if (indexes.empty() == true)
		indexes.push_back(Data("index", "index.html"));
	for (size_t i = 0; i < indexes.size(); i++)
	{
		value = getFileContent(indexes.at(i).getValue().split(), getRootPath());
		if (!value.empty())
			break ;
	}
	return (value);
}

String	Directives::getRootPath( void )
{
	if (data.getData("root").empty())
		return ("");
	String rootPath(data.getData("root").front().getValue());
	rootPath.append(path).append(rootPath.back() == '/' ? "" : "/");
	String alias(getAliasPath());
	if (!alias.empty())
		rootPath = alias;
	return (rootPath);
}

String	Directives::getAliasPath( void )
{
	vector<Data> aliases = data.getData("alias");
	if (aliases.empty())
		return ("");
	String aliasPath(aliases.front().getValue());
	aliasPath.rightTrim("/");
	return (aliasPath.append("/"));
}

ResponseHeader	Directives::returnDirective( void )
{
	ResponseHeader responseHeader;
	vector<String> values = data.getData("return").front().getValue().split();
	static map<String, String> statuses;
	if (statuses.empty())
		statuses = getStatusCode();
	/**
	 * @attention status should be dynamic.
	*/
	String statusCode;
	try
	{
		statusCode = statuses.at(values.front());
		responseHeader.status(statusCode);
		if (values.size() == 2)
			responseHeader.location(values.back());
		return (responseHeader);
	}
	catch(const std::exception& e)
	{
		statusCode = statuses["501"];
		responseHeader.status(statusCode);
		return (responseHeader);
	}
}

map<String, String> Directives::getStatusCode( void )
{
	map<String, String> status;
	status.insert(pair<String, String>("200", "200 OK"));
	status.insert(pair<String, String>("301", "301 Moved Permanently"));
	status.insert(pair<String, String>("302", "302 Found"));
	status.insert(pair<String, String>("400", "400 Bad Request"));
	status.insert(pair<String, String>("403", "403 Forbidden"));
	status.insert(pair<String, String>("404", "404 Not Found"));
	status.insert(pair<String, String>("413", "413 Content Too Large"));
	status.insert(pair<String, String>("500", "500 Internal Server Error"));
	status.insert(pair<String, String>("501", "501 Not Implemented"));
	return (status);
}
