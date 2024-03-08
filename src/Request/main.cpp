#include "Servers.hpp"
#include "webserver.h"
#include "Directives.hpp"

#define READ_SIZE 1000





// std::vector<std::string> split(std::string line, std::string sep)
// {
// 	std::vector<std::string> sp;
// 	if (line.empty())
// 		return (sp);
// 	size_t pos = 0;
	
// 	while ((pos = line.find(sep)) != std::string::npos)
// 	{
// 		std::string l = line.substr(0, pos);
// 		if (!l.empty())
// 			sp.push_back(l);
// 		pos += sep.length();
// 		line.erase(0, pos);
// 	}
// 	sp.push_back(line.substr(0, line.length()));
// 	return (sp);
// }


vector<string> split(const string& line, const string& sep) {
    vector<string> tokens;
    size_t start = 0;
	size_t end = 0;
	
    while ((end = line.find(sep, start)) != string::npos) {
		if (end != start)
        	tokens.push_back(line.substr(start, end - start));
        start = end + sep.length();
    }
    tokens.push_back(line.substr(start));
    return tokens;
}

string readF(const string &path)
{
	std::string content;
	char buffer[READ_SIZE + 1];

	int fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
		return (content);

	while (1)
	{
		ssize_t readBytes = read(fd, buffer, READ_SIZE);
		if (readBytes <= 0)
			break;
		buffer[readBytes] = 0;
		content.append(buffer, (size_t)readBytes);
	}
	close(fd);
	return (content);
}







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
	try
	{
		Parser parser(av[1]);
		Checker check(parser.getServers());
		check.fullCheck();
		ServerData serv(parser.getServers());
		// serv.displayServers();
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
	catch(exception& e)
	{
		Logger::error(cerr, e.what(), "");
	}
}
