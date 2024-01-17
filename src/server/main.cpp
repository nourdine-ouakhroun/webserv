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
	for (size_t i = 0; i < poll_fd.size(); i++)
	{
		if(poll_fd[i].revents & POLLIN)
		{
			try
			{
				servers.readyToRead(i);
			}
			catch(runtime_error &e){}
		}
		else if(poll_fd[i].revents & POLLOUT)
			servers.readyToWrite(i, poll_fd);
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
			// cout << "i'm done" << "Servers : " << servers.SocketsSize() << endl;
		}
		catch(const Servers::PollException& e)
		{
			Logger::warn(cout, e.what(), "");
			continue;
		}
	}
}




