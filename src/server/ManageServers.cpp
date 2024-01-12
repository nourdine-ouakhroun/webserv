#include"ManageServers.hpp"

ManageServers::ManageServers(ServerData	srvers)
{
	this->servers = srvers;
}

ManageServers::~ManageServers(void)
{
}

void	ManageServers::setWorkingSockets(const std::vector<SocketDependencies> & working) 
{
	this->working = working;
}

size_t	ManageServers::WorkingSocketsSize(void) const 
{
	return this->working.size();
}

const SocketDependencies&	ManageServers::getWorkingSocket(size_t index) const 
{
	return this->working[index];
}

const std::vector<SocketDependencies>&	ManageServers::getMasterSockets(void) const 
{
	return this->master;
}

const std::string&	ManageServers::getRequest(size_t index) const
{
	return this->master[index].getRequist();
}

std::string&	ManageServers::getRespond(size_t index)
{
	return this->master[index].respond;
}

void	ManageServers::initSockets(std::vector<String>&	allport)
{
	for(size_t i = 0; i < allport.size();i++)
	{
		int	fd;
		try
		{
			fd = Server::setSocket();
			std::vector<String> ipAndPort = allport[i].split(':');
			Server::bindSocket(fd, ipAndPort.front(), static_cast<int>(strtol(ipAndPort.back().c_str(), NULL, 10)));
			Server::listenPort(fd);
		}
		catch(std::runtime_error &e)
		{
			std::cout << e.what() << std::endl;
			continue;
		}
		fdSockets.push_back(fd);
	}
}

// void	ManageServers::initSocketPort80(void)
// {
// 	int	fd;
// 	try
// 	{
// 		fd = Server::setSocket();
// 		Server::bindSocket(fd, 80);
// 		Server::listenPort(fd);
// 	}
// 	catch(std::runtime_error &e)
// 	{
// 		std::cout << e.what() << std::endl;return;
// 	}
// 	fdSockets.push_back(fd);
// }

std::vector<String> removeDuplicatePorts(const std::vector<String>& allPorts)
{
	std::vector<String> uniquePorts;
	std::vector<String>::const_iterator begin = allPorts.begin();
	std::vector<String>::const_iterator end = allPorts.end();
	while (begin != end)
	{
		if (std::find(begin + 1, end, *begin) == end)
			uniquePorts.push_back(*begin);
		begin++;
	}
	return (uniquePorts);
}

std::vector<String>	ManageServers::getAllPorts(void) const
{
	std::vector<String>			allport;
	std::vector <ServerPattern>	allservers = servers.getAllServers();
	for (size_t i = 0; i < allservers.size( ); i++)
	{
		std::vector <Data> ports = allservers[i].getData("listen");
		for (size_t j = 0; j < ports.size(); j++)
		{
			allport.push_back(ports[j].getValue());
		}
	}
	// std::cout << "allport.size() : " << allport.size() << std::endl;
	allport = removeDuplicatePorts(allport);
	return allport;
}

void	ManageServers::runAllServers(void)
{
	/**
	 * @attention put allport in ManageServers header.
	*/
	allport = getAllPorts();
	initSockets(allport);
	// if (fdSockets.empty( ))
	// 	initSocketPort80( );
}


void	ManageServers::readyToWrite(size_t index)
{
	size_t send_lenght = 2000;
	if(send_lenght > working[index].respond.size())
		send_lenght =  working[index].respond.size();
	ssize_t write_value = write(working[index].getFdPoll().fd,  master[index].respond.c_str(),  send_lenght);
	if(write_value > 0)
	{
		master[index].respond.erase(0,send_lenght);
		if(master[index].respond.empty() == true)
		{
			close(working[index].getFdPoll().fd);
			erase(index);
		}
	}
}

void	ManageServers::setRespond(const std::string & respond, size_t index)
{
	this->master[index].respond = respond;
}

short	ManageServers::WorkingRevents(size_t index) const 
{
	return this->working[index].getFdPoll().revents;
}

void	ManageServers::erase(size_t index)
{
	std::vector<SocketDependencies> returnFds;
	for (size_t i = 0; i < this->master.size(); i++)
	{
		if(i != index)
			returnFds.push_back(this->master[i]);
	}
	this->master = returnFds;
}

#include <arpa/inet.h>

void ManageServers::acceptConection(size_t index)
{
	struct sockaddr_in clientSocket;
	socklen_t len = sizeof(clientSocket);

	int newfd = accept(fdSockets[index], (struct sockaddr *)&clientSocket, &len);
	if(newfd < 0)
		throw std::runtime_error("accept : filed!");
	/**
	 * @todo check fcntl fhm chno katakhd mziaaaaan
	*/
	fcntl(newfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	SocketDependencies tmp;

	tmp.ipAndPort = allport[index];

	tmp.setFdPoll(newfd, POLLIN);
	master.push_back(tmp);
}

SocketDependencies ManageServers::readyToRead(size_t i)
{
	for (size_t j = 0; j < fdSockets.size(); j++)
	{
		if(working[i].getFdPoll().fd == fdSockets[j])
		{
			acceptConection(j);
			throw std::runtime_error("accept new connection.");
		}
	}
	ReadRequest readrequest(master[i]);
	readrequest.Requist();
	master[i].setFdPoll(POLLOUT);
	return (master[i]);
}

std::vector<SocketDependencies>	ManageServers::isSocketsAreReady(void)
{
	std::vector<SocketDependencies> working = master;
	std::vector<pollfd> pworking;
	for (size_t i = 0; i < working.size(); i++)
		pworking.push_back(working[i].getFdPoll());
	int pint = poll(&pworking[0], static_cast<nfds_t>(pworking.size()), 6000);
	for (size_t i = 0; i < pworking.size(); i++)
		working[i].setFdPoll(pworking[i]);
	if(pint == 0)
		throw ManageServers::PollException("Server reloaded");
	if(pint < 0)
		throw std::runtime_error("poll : poll was failed");
	return working;
}

void	ManageServers::setMasterSockets(void)
{
	for (size_t i = 0; i < fdSockets.size(); i++)
	{
		SocketDependencies tmp;
		tmp.setFdPoll(fdSockets[i], POLLIN);
		master.push_back(tmp);
	}
}
