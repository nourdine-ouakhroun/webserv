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


#include"ManageServers.hpp"
#include"Directives.hpp"


ResponseHeader	to_do(GeneralPattern& __unused targetInfo, __unused String path)
{
	String root;
	String fileName;
	Directives directive(targetInfo, path);
	ResponseHeader responseHeader;

	directive.handleLogges();


	/**
	 * @brief getRootPath : this function checks the root and alias exists and return it.
	 * @name root and alias directives.
	 * @details In Nginx, when you have both the root directive and the alias directive within the same location block, the alias directive takes precedence over the root directive.
	 * 	1. alias
	 * 	2. root
	*/
	root = directive.getRootPath();

	
	/**
	 * @brief the priority for serving files is determined by the directives used in the configuration.
	 * 	1. try_files
	 * 	2. index
	 * 	3. auto_index on
	*/
	if (!targetInfo.getData("try_files").empty())
	{
		try
		{
			fileName = directive.tryFiles();
			if (!fileName.empty())
				responseHeader.fileName(fileName);
		}
		catch (String &e)
		{
			responseHeader.status("301 Moved Permanently").location(e);
		}
		return (responseHeader);
	}

	fileName = directive.indexing();
	if (!fileName.empty())
		return (responseHeader.fileName(fileName));

	if (!targetInfo.getData("return").empty())
		return (directive.returnDirective());

	if (targetInfo.isExist(Data("autoindex", "on")))
		return (directive.autoIndexing());

	return (directive.errorPage("404", "Not Found"));
}

ResponseHeader	handler(ServerPattern& server, GeneralPattern __unused &model)
{
	ResponseHeader	responseHeader;
	String method = model.getData("Method").front().getValue();
	String path(method.split()[1]);
	path.trim(" \t\r\n");

	vector<Data> roots = server.getData("root");
	String root;
	if (roots.empty() == false)
		root = roots.front().getValue();
	
	if (server.checkIsDirectory(root.append(path)) == 0) // check is url is a file.
		return (responseHeader.fileName(root));

	if (path.back() != '/') // redirect the path that doesn't containt '/' in the end.
		return (responseHeader.status("301 Moved Permanently").location(path + "/"));

	path.rightTrim("/");

	LocationPattern	loca = ServerPattern::getLocationByPath(server.getLocation(), path);
	GeneralPattern target;

	try
	{
		target = dynamic_cast<GeneralPattern&>(loca);
		if (loca.getPath().empty() && path.equal("/"))
			target = dynamic_cast<GeneralPattern&>(server);
	}
	catch(const exception& e)
	{
		cerr << e.what() << '\n';
	}

	return (to_do(target, loca.getPath()));
}


void	socketHaveEvent(ManageServers &Manageservers, ServerData& serv)
{
	for (size_t i = 0; i < Manageservers.WorkingSocketsSize(); i++)
	{
		if(Manageservers.WorkingRevents(i) & POLLIN)
		{
			try
			{
				SocketDependencies socket = Manageservers.readyToRead(i);
				cout << Manageservers.getRequest(i) << endl;
				GeneralPattern header(Parser::parseHeader(Manageservers.getRequest(i)));
				vector<ServerPattern> server = ServerData::getServer(serv, socket.ipAndPort, header.getData("Host").front().getValue());
				ResponseHeader response = handler(server.front(), header);
				String filename = response.getFileName();
				if (filename.empty() == false)
				{
					String* str = getContentFile(filename);
					if (!str)
						throw (exception());
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
				Manageservers.setRespond(*str, i);
				delete str;
				// Manageservers.setRespond("HTTP/1.1 200 OK\r\n\r\n <h1> hello </h1>", i);
			}
			catch(runtime_error &e){}
		}
		else if(Manageservers.WorkingRevents(i) & POLLOUT)
			Manageservers.readyToWrite(i);
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
	ManageServers Manageservers(serv);
	serv.displayServers();
	Manageservers.runAllServers();
	Manageservers.setMasterSockets();
	while (true)
	{
		try{
			Manageservers.setWorkingSockets(Manageservers.isSocketsAreReady());
			socketHaveEvent(Manageservers, serv);
		}
		catch(const ManageServers::PollException& e)
		{
			Logger::warn(cout, e.what(), "");
			continue;
		}
	}
}




