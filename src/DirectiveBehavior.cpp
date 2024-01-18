#include "webserver.h"
#include <map>

void	handleLogges(GeneralPattern& server)
{
	static int stdout_fd;
	static int stderr_fd;

	/**
	 * @attention possible fd leaks here.
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

	vector<Data> accessLog = server.getData("access_log");
	vector<Data> errorLog = server.getData("error_log");
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

ResponseHeader	errorPage(vector<Data> errorPages, String errorNumber, String message)
{
	ResponseHeader responseHeader;
	for (size_t i = 0; i < errorPages.size(); i++)
	{
		vector<String> numbers = errorPages.at(i).getValue().split();
		for (size_t i = 0; i < numbers.size() - 1; i++)
			if (!numbers.at(i).compare(errorNumber))
				return (responseHeader.status("302	Found").location(*(numbers.end() - 1)));
	}
	responseHeader.status(errorNumber + " " + message).body(new String("<h1>" + errorNumber + " " + message + "</h1>"));
	return (responseHeader);
}

ResponseHeader	autoIndexing(GeneralPattern& loca, const String& dir, String	path)
{
	String content;
	String file;
	ResponseHeader responseHeader;

	String str = getDirectoryContent(dir, path);
	if (str.empty())
		return (errorPage(loca.getData("error_page"), "404", "Not Found"));
	return (responseHeader.body(new String(str)));
}

String	tryFiles(const vector<String>& files, const String& path)
{
	String value;
	for (size_t i = 0; i < files.size() - 1; i++)
	{
		String tmp(files.at(i));
		String::size_type pos = tmp.find("$uri");
		if (pos != String::npos)
			tmp.replace(pos, 4, path);
		if (checkFile(tmp) == true)
			return (tmp);
	}
	return (value);
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

map<String, String> getStatusCode( void )
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

ResponseHeader	returnDirective(const vector<Data> &returns, ResponseHeader &responseHeader)
{
	vector<String> values = returns.front().getValue().split();
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