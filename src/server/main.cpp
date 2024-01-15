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
void	socketHaveEvent(Servers &servers, vector<pollfd> &poll_fd)
{
	for (size_t i = 0; i < servers.SocketsSize(); i++)
	{
		if(poll_fd[i].revents & POLLIN)
		{
			cout << "read " << "servers : " << servers.SocketsSize() << endl;
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
			cout << "i'm done" << "Servers : " << servers.SocketsSize() << endl;
		}
		catch(const Servers::PollException& e)
		{
			Logger::warn(cout, e.what(), "");
			continue;
		}
	}
}




