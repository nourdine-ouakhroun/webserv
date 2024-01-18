// #include "Server.hpp"
// #include "Parser.hpp"
// #include "Checker.hpp"
// #include "ServerData.hpp"



// int main(int ac, char *av[])
// {
// 	if (ac != 2)
// 		return (0);
// 	Parser parsing(av[1]);
// 	Checker	check(parsing.getServers());
// 	check.fullCheck();
// 	ServerData servers(parsing.getServers());
	
// 	Server	s;
// }

// #include "ManageServers.hpp"
#include "Servers.hpp"
#include "webserver.h"
#include "Directives.hpp"

void socketHaveEvent(Servers &servers, vector<pollfd> &poll_fd)
{
	for (size_t i = 0; i < poll_fd.size(); i++)
	{
		if (poll_fd[i].revents & POLLIN)
		{
			try
			{
				servers.readyToRead(i, poll_fd);
			}
			catch (runtime_error &e)
			{
				Logger::error(cerr, e.what(), "");
			}
		}
		else if (poll_fd[i].revents & POLLOUT)
			servers.readyToWrite(i, poll_fd);
	}
}

int main(int ac, char **av)
{
	// static_cast<void>(ac);
	if (ac != 2)
	{
		Logger::error(std::cerr, "invalid argements.", "");
		return (1);
	}
	signal(SIGPIPE, SIG_IGN);
	Parser parser(av[1]);
	ServerData serv(parser.getServers());
	Servers servers(serv);
	servers.runAllServers();
	servers.setMasterSockets();
	while (true)
	{
		try
		{
			vector<pollfd> poll_fd;
			servers.isSocketsAreReady(poll_fd);
			socketHaveEvent(servers, poll_fd);
			// cout << "i'm done" << "Servers : " << servers.SocketsSize() << endl;
		}
		catch (const Servers::PollException &e)
		{
			Logger::warn(cout, e.what(), "");
			continue;
		}
	}
}
