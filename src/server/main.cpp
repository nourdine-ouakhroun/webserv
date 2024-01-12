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

void	socketHaveEvent(ManageServers &Manageservers, ServerData& serv)
{
	for (size_t i = 0; i < Manageservers.WorkingSocketsSize(); i++)
	{
		if(Manageservers.WorkingRevents(i) & POLLIN)
		{
			try
			{
				SocketDependencies socket = Manageservers.readyToRead(i);
				std::cout << Manageservers.getRequest(i) << std::endl;
				GeneralPattern header(Parser::parseHeader(Manageservers.getRequest(i)));
				std::cout << ">>>>> Ip Address: " << socket.ipAndPort << std::endl;
				std::cout << ">>>>> Host: " << header.getData("Host").front().getValue() << std::endl;
				std::vector<ServerPattern> server = ServerData::getServer(serv, socket.ipAndPort, header.getData("Host").front().getValue());
				ServerPattern::printServerPatternInfo(server.front());
				Manageservers.setRespond("HTTP/1.1 200 OK\r\n\r\n <h1> hello </h1>", i);
			}
			catch(std::runtime_error &e){}
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
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	ServerData serv(parser.getServers());
	ManageServers Manageservers(serv);

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
			Logger::warn(std::cout, e.what(), "");
			continue;
		}
	}
}




