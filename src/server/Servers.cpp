#include "Servers.hpp"
#include "../Request/Request.hpp"
#include "../Request/Response.hpp"

#define READ_SIZE 1000
std::string readFile(const std::string &path)
{
	std::string	content;
	char		buffer[READ_SIZE + 1];

	int fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
		return (content);

	while (1)
	{
		ssize_t readBytes = read(fd, buffer, READ_SIZE);
		if (readBytes <= 0)
			break;
		buffer[readBytes] = 0;
		content.append(buffer, (u_long)readBytes);
	}
	close(fd);
	return (content);
}

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

void	Servers::initSockets(vector<String>&	allport)
{
	for(size_t i = 0; i < allport.size();i++)
	{
		int	fd;
		try
		{
			fd = Server::setSocket();
			vector<String> infos = allport[i].split(':');
			Server::bindSocket(fd, infos.front(), (int)strtol(infos.back().c_str(), NULL, 10));
			Server::listenPort(fd);
		}
		catch(runtime_error &e)
		{
			cout << e.what() << endl;continue;
		}
		fdSockets.push_back(fd);
	}
}

// void	Servers::initSocketPort80(void)
// {
// 	int	fd;
// 	try
// 	{
// 		fd = Server::setSocket();
// 		Server::bindSocket(fd, 80);
// 		Server::listenPort(fd);
// 	}
// 	catch(runtime_error &e)
// 	{
// 		cout << e.what() << endl;return;
// 	}
// 	fdSockets.push_back(fd);
// }

vector<String> removeDuplicatePorts(const vector<String>& allPorts)
{
	vector<String> uniquePorts;
	vector<String>::const_iterator begin = allPorts.begin();
	vector<String>::const_iterator end = allPorts.end();
	while (begin != end)
	{
		if (find(begin + 1, end, *begin) == end)
			uniquePorts.push_back(*begin);
		begin++;
	}
	return (uniquePorts);
}

vector<String>	Servers::getAllPorts(void) const
{
	vector<String>			allport;
	vector <ServerPattern>	allservers = servers.getAllServers();
	for (size_t i = 0; i < allservers.size( ); i++)
	{
		vector <Data> ports = allservers[i].getData("listen");
		for (size_t j = 0; j < ports.size(); j++)
		{
			allport.push_back(ports[j].getValue());
		}
	}
	// cout << "allport.size() : " << allport.size() << endl;
	allport = removeDuplicatePorts(allport);
	return allport;
}

void	Servers::runAllServers(void)
{
	/**
	 * @attention put allport in ManageServers header.
	*/
	allport = getAllPorts();
	initSockets(allport);
}


void	Servers::readyToWrite(size_t &index, vector<pollfd> &poll_fd)
{
	// size_t send_lenght = 2000;
	// if(send_lenght > master[index].respond.size())
	// 	send_lenght =  master[index].respond.size();
	ssize_t write_value = write(master[index].getFdPoll().fd, master[index].respond.c_str(), master[index].respond.size());
	if(write_value > 0)
	{
		master[index].respond.erase(0, (size_t)write_value);
		if(master[index].respond.empty() == true)
		{
			close(master[index].getFdPoll().fd);
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
	tmp.ipAndPort = allport[index];
	master.push_back(tmp);
}

void Servers::readyToRead(size_t i, vector<pollfd> &poll_fd)
{
	for (size_t j = 0; j < fdSockets.size(); j++)
	{
		if(poll_fd[i].fd == fdSockets[j])
		{
			acceptConection(j);
			return;
		}
	}


	Request 	request;
	Response	response;

	std::string fileToServe;

	ReadRequest read_request(master[i]);
	read_request.Request();
	cout << "size : " << master[i].request.size() << endl;
	request.parseRequest(master[i].request);
	request.parseBody(request.getBody());

	ServerPattern server = ServerData::getServer(this->servers, master[i].ipAndPort, request.header("Host")).front();
	try
	{

		fileToServe = request.checkServer(server);
		response.setFileToServe(fileToServe);
		std::string body = request.getBody();
		std::cout << "body" <<  body << endl;
		// response.setRequest(request);

		std::cout << "fileToServe " << fileToServe << std::endl;
	}
	catch (int status)
	{
		response.setResponse( status );
	}

	std::ifstream fileStream(fileToServe, std::ifstream::ate | std::ifstream::binary);
	std::string contentLength = std::to_string(fileStream.tellg());
	fileStream.close();

	response.setBody(readFile(fileToServe));

	response.setHeader("Server", "nginx-v2");
	response.setHeader("Content-Type", response.getMimeType(request.extention(fileToServe)));
	response.setHeader("Content-Length", contentLength);
	response.makeResponse();


	master[i].respond = response.getResponse();
	master[i].setFdPoll(POLLOUT);

	
}

void Servers::isSocketsAreReady(vector<pollfd> &poll_fd)
{
	for (size_t i = 0; i < master.size(); i++)
	{
		poll_fd.push_back(master[i].getFdPoll());
	}
	int pint = poll(&poll_fd[0], static_cast<nfds_t>(poll_fd.size()), 1000);
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