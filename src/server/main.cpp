/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nouakhro <nouakhro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 16:41:57 by nouakhro          #+#    #+#             */
/*   Updated: 2023/11/28 16:48:52 by nouakhro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include"Directives.hpp"
#include"Servers.hpp"

ResponseHeader servingFileContent(GeneralPattern& targetInfo, String path)
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

ResponseHeader	to_do(GeneralPattern& targetInfo, String path, String &model)
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

ResponseHeader	handler(ServerPattern& server, GeneralPattern &model)
{
	ResponseHeader	responseHeader;
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
			break ;
		}
		if (!strncmp(pathss[i].c_str(), test.c_str(), pathss[i].size()))
			newPaths.push_back(pathss[i]);
	}

	vector<String>::iterator it = max_element(newPaths.begin(), newPaths.end());
	if (it == newPaths.end())
		return (responseHeader);
	cout << "Larget string : " << *it << endl;
	LocationPattern	loca = ServerPattern::getLocationByPath(server.getLocations(), *it);
	GeneralPattern target;

	try
	{
		target = dynamic_cast<GeneralPattern&>(loca);
		if (loca.getPath().empty() && path.equal("/") && path.size() == 1)
			target = dynamic_cast<GeneralPattern&>(server);
	}
	catch(const exception& e)
	{
		cerr << e.what() << '\n';
	}
	return (to_do(target, loca.getPath(), path));
}

void	socketHaveEvent(Servers &servers, vector<pollfd> &poll_fd)
{
	for (size_t i = 0; i < poll_fd.size(); i++)
	{
		if(poll_fd[i].revents & POLLIN)
		{
			try
			{
				servers.readyToRead(i, poll_fd);
			}
			catch(runtime_error &e){}
		}
		else if(poll_fd[i].revents & POLLOUT)
			servers.readyToWrite(i, poll_fd);
	}
}

void	sighandler(int signum)
{
	(void)signum;
}

int main(int ac, char **av)
{
	static_cast<void>(ac);
	signal(SIGPIPE, sighandler);

	Parser parser(av[1]);
	try
	{
		Checker checker(parser.getServers());
		checker.fullCheck();		
	}
	catch(const exception& e)
	{
		cerr << e.what() << '\n';
	}
	ServerData serv(parser.getServers());
	Servers servers(serv);
	servers.runAllServers();
	servers.setMasterSockets();
	while (true)
	{
		try{
			vector<pollfd> poll_fd;
			servers.isSocketsAreReady(poll_fd);
			socketHaveEvent(servers, poll_fd);
			// cout << "i'm done" << "Servers : " << servers.SocketsSize() << endl;
		}
		catch(const Servers::PollException& e)
		{
			Logger::warn(cout, e.what(), "");
			continue;
		}
	}
}
