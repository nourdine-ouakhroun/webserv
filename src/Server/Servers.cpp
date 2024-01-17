#include"Servers.hpp"

Servers::Servers(ServerData	srvers)
{
	this->servers = srvers;
}

Servers::~Servers(void)
{
}

// void	Servers::setWorkingSockets(const vector<Socket> & working) 
// {
// 	this->working = working;
// }

size_t	Servers::SocketsSize(void) const 
{
	return this->master.size();
}

const Socket&	Servers::getSocket(size_t index) const 
{
	return this->master[index];
}

const vector<Socket>&	Servers::getMasterSockets(void) const 
{
	return this->master;
}

const string&	Servers::getHeader(size_t index) const
{
	return this->master[index].getHeader();
}

// string&	Servers::getRespond(size_t index)
// {
// 	return this->master[index].respond;
// }

void	Servers::initSockets(vector<int>&	allport)
{
	for(size_t i = 0; i < allport.size();i++)
	{
		int	fd;
		try
		{
			fd = Server::setSocket();
			Server::bindSocket(fd, allport[i]);
			Server::listenPort(fd);
		}
		catch(runtime_error &e)
		{
			cout << e.what() << endl;continue;
		}
		fdSockets.push_back(fd);
	}
}

void	Servers::initSocketPort80(void)
{
	int	fd;
	try
	{
		fd = Server::setSocket();
		Server::bindSocket(fd, 80);
		Server::listenPort(fd);
	}
	catch(runtime_error &e)
	{
		cout << e.what() << endl;return;
	}
	fdSockets.push_back(fd);
}

vector<int>	Servers::getAllPorts(void) const
{
	vector<int>			allport;
	vector <ServerModel>	allservers = servers.getAllServers();
	for (size_t i = 0; i < allservers.size( ); i++)
	{
		vector <Data> ports = allservers[i].getData("listen");
		for (size_t j = 0; j < ports.size(); j++)
		{
			allport.push_back(static_cast<int>(strtol(ports[j].getValue().c_str(),NULL, 10)));
		}
	}
	return allport;
}

void	Servers::runAllServers(void)
{
	vector< int >	allport = getAllPorts();
	initSockets(allport);
	if (fdSockets.empty( ))
		initSocketPort80( );
}


void	Servers::readyToWrite(size_t &index, vector<pollfd> &poll_fd)
{
	cout << "index : " << index << " master : " << master.size() << endl;
	size_t send_lenght = 2000;
	if(send_lenght > master[index].respond.size())
		send_lenght =  master[index].respond.size();
	ssize_t write_value = write(master[index].getFdPoll().fd,  master[index].respond.c_str(),  send_lenght);
	if(write_value > 0)
	{
		master[index].respond.erase(0,send_lenght);
		if(master[index].respond.empty() == true)
		{
			close(master[index].getFdPoll().fd);
			cout << "erase : " << index << endl;
			erase(index, master);
			erase(index, poll_fd);
			index--;
		}
	}
}

// void	Servers::setRespond(const string & respond, size_t index)
// {
// 	this->master[index].respond = respond;
// }

short	Servers::Revents(size_t index) const 
{
	return this->master[index].getFdPoll().revents;
}


void Servers::acceptConection(size_t index)
{
	int newfd = accept(fdSockets[index], NULL, NULL);
	if(newfd < 0)
		throw runtime_error("accept : filed!");
	/**
	 * @todo check fcntl fhm chno katakhd mziaaaaan
	*/
	fcntl(newfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	Socket tmp;
	tmp.setFdPoll(newfd, POLLIN);
	master.push_back(tmp);
}

void Servers::readyToRead(size_t i)
{
	cout << " i : " << i << " size : " << master.size() << endl;
	for (size_t j = 0; j < fdSockets.size(); j++)
	{
		if(master[i].getFdPoll().fd == fdSockets[j])
		{
			acceptConection(j);
			return;
		}
	}
	try
	{
		// Read the request;
		ReadRequest read_request(master[i]);
		read_request.Request();
	}
	catch(int)
	{
		cout << "Change permission" << endl;
		master[i].respond = "HTTP/1.1 200 OK\r\n\r\n <h1> hello </h1>";
		cout << master[i].getHeader() << endl;
		cout << " /////////////////// " << endl;
		cout << master[i].getBody() << endl;
		// Change read permission to write permission;
		master[i].setFdPoll(POLLOUT);
	}
	
}

void Servers::isSocketsAreReady(vector<pollfd> &poll_fd)
{
	for (size_t i = 0; i < master.size(); i++)
	{
		poll_fd.push_back(master[i].getFdPoll());
	}
	int pint = poll(&poll_fd[0], static_cast<nfds_t>(poll_fd.size()), 6000);
	cout << "poll_fd.size() : " << poll_fd.size() << ", "<<  master.size() << endl;
	if(pint == 0)
		throw Servers::PollException("Server reloaded");
	if(pint < 0)
		throw runtime_error("poll : poll was failed");
}

void	Servers::setMasterSockets(void)
{
	for (size_t i = 0; i < fdSockets.size(); i++)
	{
		Socket tmp;
		tmp.setFdPoll(fdSockets[i], POLLIN);
		master.push_back(tmp);
	}
}
