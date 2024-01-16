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


#include"Servers.hpp"

// ResponseHeader	to_do(GeneralPattern& __unused targetInfo, __unused String path)
// {
// 	String root;
// 	String fileName;
// 	Directives directive(targetInfo, path);
// 	ResponseHeader responseHeader;

// 	// directive.handleLogges();

// 	root = directive.getRootPath();

// 	if (!targetInfo.getData("try_files").empty())
// 		return (directive.tryFiles());

// 	fileName = directive.indexing();
// 	if (!fileName.empty())
// 		return (responseHeader.fileName(fileName));

// 	if (!targetInfo.getData("return").empty())
// 		return (directive.returnDirective());

// 	if (targetInfo.isExist(Data("autoindex", "on")))
// 		return (directive.autoIndexing());

// 	return (directive.errorPage("404", "Not Found"));
// }

// ResponseHeader	handler(ServerPattern& server, GeneralPattern __unused &model)
// {
// 	ResponseHeader	responseHeader;
// 	String method = model.getData("Method").front().getValue();
// 	String path(method.split()[1]);
// 	path.trim(" \t\r\n");

// 	vector<Data> roots = server.getData("root");
// 	// vector<Data> roots;
// 	// roots.push_back(Data("root", "project/gamestore"));
// 	String root;
// 	if (roots.empty() == false)
// 		root = roots.front().getValue();
	
// 	if (server.checkIsDirectory(root.append(path)) == 0) // check is url is a file.
// 		return (responseHeader.fileName(root));

// 	if (path.back() != '/') // redirect the path that doesn't containt '/' in the end.
// 		return (responseHeader.status("301 Moved Permanently").location(path + "/"));



// 	path.rightTrim("/");

// 	LocationPattern	loca = ServerPattern::getLocationByPath(server.getLocation(), path);
// 	GeneralPattern target;

// 	try
// 	{
// 		target = dynamic_cast<GeneralPattern&>(loca);
// 		if (loca.getPath().empty() && path.equal("/"))
// 			target = dynamic_cast<GeneralPattern&>(server);
// 	}
// 	catch(const exception& e)
// 	{
// 		cerr << e.what() << '\n';
// 	}

// 	return (to_do(target, loca.getPath()));
// }

void	socketHaveEvent(Servers &servers, vector<pollfd> &poll_fd)
{
	for (size_t i = 0; i < poll_fd.size(); i++)
	{
		if(poll_fd[i].revents & POLLIN)
		{
			// cout << "read " << "servers : " << servers.SocketsSize() << endl;
			try
			{
				servers.readyToRead(i);
				// cout << servers.getHeader(i) << endl;
			}
			catch(runtime_error &e){}
		}
		else if(poll_fd[i].revents & POLLOUT)
			servers.readyToWrite(i);
	}
}

int main(int ac, char **av)
{
	static_cast<void>(ac);
	
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
