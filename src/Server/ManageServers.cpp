#include"ManageServers.hpp"

ManageServers::ManageServers(ServerData	srvers)
{
	this->servers = srvers;
}


void ManageServers::initSockets(std::vector<int> &allport)
{
	for(size_t i = 0; i < allport.size();i++)
	{
		int fd;
		try{
			fd = Server::setSocket();
			Server::bindSocket(fd, allport[i]);
			Server::listenPort(fd);
		}
		catch(std::runtime_error &e){std::cout << e.what() << std::endl;continue;}
		fdSockets.push_back(fd);
	}
}
void ManageServers::initSocketPort80()
{
	int fd;
	try{
		fd = Server::setSocket();
		Server::bindSocket(fd, 80);
		Server::listenPort(fd);
	}
	catch(std::runtime_error &e){std::cout << e.what() << std::endl;return;}
	fdSockets.push_back(fd);
}
std::vector<int> ManageServers::getAllPorts() const
{
	std::vector< int >	allport;
	std::vector < ServerModel > allservers = servers.getAllServers();
	for (size_t i = 0; i < allservers.size( ); i++)
	{
		std::vector <Data> ports = allservers[i].getData("listen");
		for (size_t j = 0; j < ports.size(); j++)
		{
			allport.push_back(static_cast<int>(strtol(ports[j].getValue().c_str(),NULL, 10)));
		}
	}
	return allport;
}
void	ManageServers::runAllServers()
{
	std::vector< int >	allport = getAllPorts();
	initSockets(allport);
	if (fdSockets.empty( ))
		initSocketPort80( );
}

void ManageServers::setWorkingSockets(const std::vector<FileDependency> & working) 
{
	this->working = working;
}
size_t ManageServers::WorkingSocketsSize() const 
{
	return this->working.size();
}
const FileDependency & ManageServers::getWorkingSocket(size_t index) const 
{
	return this->working[index];
}
const std::vector<FileDependency> & ManageServers::getMasterSockets() const 
{
	return this->master;
}
const std::string & ManageServers::getRequest(size_t index) const
{
	return this->master[index].getRequist();
}
std::string & ManageServers::getRespond(size_t index)
{
	return this->master[index].respond;
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
short ManageServers::WorkingRevents(size_t index) const 
{
	return this->working[index].getFdPoll().revents;
}

void ManageServers::erase(size_t index)
{
	std::vector<FileDependency> returnFds;
	for (size_t i = 0; i < this->master.size(); i++)
	{
		if(i != index)
			returnFds.push_back(this->master[i]);
	}
	this->master = returnFds;
}

int headerMethod(String line)
{
	if(line.empty() == true)
		throw std::runtime_error("");
	std::vector<String> splited = line.split();
	/**
	 * @todo protection
	*/
	if(splited[0] == "GET")
		return 0;
	if(splited[0] == "POST")
		return 1;
	if(splited[0] == "DELET")
		return 2;
	return 3;
}


void methodSerch(FileDependency &file, std::string request)
{
	size_t pos = request.find("\r\n");
	if(pos != NPOS)
	{
		file.setMethod(headerMethod(request.substr(0, pos)));
		return;
	}
	throw std::runtime_error("");
}

void setOnlyHeadre(FileDependency &file, std::string request)
{
	size_t	pos = request.find("\r\n\r\n");
	if(pos == NPOS)
		throw std::runtime_error("");
	file.setRequist(request.substr(0, pos + 4));
	file.rest = request.substr(pos + 4);
	if(file.getMethod() == POST)
	{
		size_t	pos = file.getRequist().find("Content-Length: ");
		if(pos == NPOS)
			return ;
		long lenght = strtol(file.getRequist().substr(pos + 16).c_str(), NULL, 10);
		file.setContenlenght(static_cast<ssize_t>(lenght));
		pos = file.getRequist().find("boundary=");
		if(pos != NPOS)
		{
			std::string boundary = file.getRequist().substr(pos + 9, file.getRequist().find("\r\n", pos) - (pos + 9));
			boundary.insert(0, "--");
			file.setBoundary(boundary);
		}
	}
	file.setLenght(0);
}
void putInString(FileDependency &file)
{
	size_t	begin;

	if((begin = file.rest.find(file.getBoundary())) != NPOS)
	{
		size_t	end = file.rest.rfind("\r\n", begin);
		if(end == NPOS)
			end = 0;
		file.setRequist(file.rest.substr(0, end));
		file.appendLenght(end);
		file.rest.erase(0, end);
		file.status = DEFAULT;
		return ;
	}
	size_t	end = file.rest.rfind("\r\n");
	if(end == NPOS || end == 0)
		throw std::runtime_error("");
	file.setRequist(file.rest.substr(0, end));
	file.appendLenght(end);
	file.rest.erase(0, end);
}
void putInFile(FileDependency &file)
{
	size_t	begin;
	if((begin = file.rest.find(file.getBoundary())) != NPOS)
	{
		size_t	end = file.rest.rfind("\r\n", begin);
		if(end == NPOS)
			end = 0;
		file.appendLenght(end);
		write(file.getFd(), file.rest.substr(0, end).c_str(), file.rest.substr(0, end).size());
		close(file.getFd());
		file.rest.erase(0, end);
		file.status = DEFAULT;
		return;
	}
	size_t	end = file.rest.rfind("\r\n");
	if(end == NPOS || end == 0)
		throw std::runtime_error("");
	write(file.getFd(), file.rest.substr(0, end).c_str(), file.rest.substr(0, end).size());
	file.appendLenght(end);
	file.rest.erase(0, end);
	return;
}

void checkIfFile(FileDependency &file, size_t	&begin)
{
	begin = file.rest.rfind("\r\n", begin);
	if(begin == NPOS)
		begin = 0;
	size_t	end = file.rest.find("\r\n\r\n", begin + 2);
	if(end == NPOS)
		end = file.rest.size();
	else
		end += 4;
	std::string tmp_string(file.rest.substr(begin,  end  - begin));
	file.rest.erase(begin, end  - begin);
	file.appendLenght((end  - begin));
	size_t	pos = tmp_string.find("filename=\"", begin);
	if(pos != NPOS)
	{
		std::string filename(tmp_string.substr(pos + 10, tmp_string.find("\"", pos + 10) - (pos + 10)));
		file.setFileName(filename);
		int	fd = open(filename.c_str(), O_CREAT | O_RDWR | O_APPEND, 0777);
		file.setFd(fd);
		file.status = PUTINFILE;
		return ;
	}
	file.setRequist(tmp_string);
	if(file.getRequist().find(file.getBoundary() + "--") != NPOS)
	{
		file.appendLenght(file.rest.size());
		file.setRequist(file.rest);
		file.rest.erase(0, file.rest.size());
		return;
	}
	file.status = PUTINSTRING;
	return;
}

void removePartOfupload(FileDependency &file, std::string &request)
{
	size_t	begin;
	if(file.getBoundary().empty() == false)
	{
		if(file.status == PUTINSTRING)
			putInString(file);
		else if(file.status == PUTINFILE)
			putInFile(file);
		else if(file.status == DEFAULT && (begin = file.rest.find(file.getBoundary()) != NPOS))
			checkIfFile(file, begin);
	}
	else
	{
		file.setRequist(request);
		file.appendLenght(request.size());
	}
}

void	readRequist(FileDependency &file)
{
	ssize_t		bytes;
	String		boundary;
	char		buffer[READ_NUMBER];

	bytes = 0;
	memset(buffer, 0, READ_NUMBER);
	bytes = recv(file.getFdPoll().fd, buffer, READ_NUMBER - 1, 0);
	if(bytes == 0)
	{
		file.setContenlenght(file.getLenght());
		file.rest.clear();
	}
	else if(bytes > 0)
	{
		if(bytes < 0)
			bytes = 0;
		std::string request (buffer, (size_t)bytes);
		request.insert(0, file.rest);
		file.rest = request;

		while (!file.rest.empty())
		{
			if(file.getMethod() == -1)
				methodSerch(file, request);
			else if(file.getRequist().empty() == true)
				setOnlyHeadre(file, request);
			else if(file.getMethod() == POST)
				removePartOfupload(file, request);		
		}
	}
	if(file.getLenght() != file.getContenlenght() || !file.rest.empty())
		throw std::runtime_error("");
	return ;
}
void ManageServers::readyToRead(size_t i)
{
	for (size_t j = 0; j < fdSockets.size(); j++)
	{
		if(working[i].getFdPoll().fd == fdSockets[j])
		{
			int newfd = accept(fdSockets[j], NULL, NULL);
			if(newfd < 0)
				throw std::runtime_error("accept : filed!");
			/**
			 * @todo check fcntl fhm chno katakhd mziaaaaan
			*/
			fcntl(newfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
			FileDependency tmp;
			tmp.setFdPoll(newfd, POLLIN);
			master.push_back(tmp);
			return;
		}
	}
	readRequist(master[i]);
	master[i].setFdPoll(POLLOUT);

}
std::vector<FileDependency>	ManageServers::isSocketsAreReady()
{
	std::vector<FileDependency> working = master;
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


void	ManageServers::acceptConection()
{
	for (size_t i = 0; i < fdSockets.size(); i++)
	{
		FileDependency tmp;
		tmp.setFdPoll(fdSockets[i], POLLIN);
		master.push_back(tmp);
	}
}
ManageServers::~ManageServers()
{
}