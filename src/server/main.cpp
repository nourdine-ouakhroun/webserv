/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeroual <mzeroual@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 16:41:57 by nouakhro          #+#    #+#             */
/*   Updated: 2024/01/18 12:12:47 by mzeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include"Directives.hpp"
#include"Servers.hpp"



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
			catch(runtime_error &e)
			{
				Logger::error(cerr, e.what(), "");
			}
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
