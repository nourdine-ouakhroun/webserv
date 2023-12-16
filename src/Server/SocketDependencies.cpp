#include"SocketDependencies.hpp"


SocketDependencies::SocketDependencies(/* args */)
{
    contenlenght = 0;
    lenght = -1;
	method = -1;
	status = 0;
}
// SocketDependencies::SocketDependencies(const SocketDependencies&	socket)
// {
// 	*this = socket;
// }
// SocketDependencies &SocketDependencies::operator=(const SocketDependencies&	socket)
// {
// 	if(this != &socket)
// 	{
// 		this->boundary = socket.boundary;
// 		this->request = socket.request;
// 		this->filename = socket.filename;
// 		this->lenght = socket.lenght;
// 		this->contenlenght = socket.contenlenght;
// 		this->fd = socket.fd;
// 		this->method = socket.method;
// 		this->fdpoll = socket.fdpoll;
// 		this->status = socket.status;
// 		this->respond = socket.respond;
// 		this->rest = socket.rest;
// 	}
// 	return *this;
// }

SocketDependencies::~SocketDependencies()
{
}
// void SocketDependencies::eraseRequist(size_t _n, size_t linght)
// {
// 	this->requist.erase(_n, linght);
// }

// const std::string &SocketDependencies::getRequist() const 
// {
// 	return requist;
// }
// const int &SocketDependencies::getStatus() const
// {
// 	return status;
// }
const int &			SocketDependencies::getMethod() const
{
	return this->method;
}
void 			SocketDependencies::setMethod(const int & method)
{
	this->method = method;
}
void			SocketDependencies::setFileName(const std::string &filename)
{
	this->filename = filename;
}
const std::string&	SocketDependencies::getFileName() const
{
	return filename;
}
const std::string & SocketDependencies::getRequist() const
{
	return this->request;
}

// void SocketDependencies::setRequist(const std::string& request,const size_t& lenght) 
// {
// 	this->request.append(request, this->request.size(), lenght);
// }
void SocketDependencies::setRequist(const std::string& request) 
{
	this->request.append(request);
}
void SocketDependencies::setFdPoll(const int &fd, const short & events)
{
	this->fdpoll.fd = fd;
	this->fdpoll.events = events;
}
void SocketDependencies::setFdPoll(const short & events)
{
	this->fdpoll.events = events;
}
void SocketDependencies::setFdPoll(const pollfd &poll_struct)
{
	this->fdpoll = poll_struct;
}

const pollfd &SocketDependencies::getFdPoll() const
{
    return this->fdpoll;
}

const std::string &SocketDependencies::getBoundary() const
{
	return this->boundary;
}
const ssize_t& SocketDependencies::getContenlenght() const
{
	return this->contenlenght;
}
const ssize_t & SocketDependencies::getLenght() const
{
	return this->lenght;
}
void	SocketDependencies::appendLenght(const size_t & lenght)
{
	this->lenght += (ssize_t)lenght;

}
void SocketDependencies::setLenght(const size_t & lenght)
{
	this->lenght = (ssize_t)lenght;
}
void SocketDependencies::setContenlenght(const ssize_t & contenlenght)
{
	this->contenlenght = contenlenght;
}
void SocketDependencies::setBoundary(const std::string &	boundary)
{
	this->boundary = boundary;
}

const int	&SocketDependencies::getFd() const
{
	return this->fd;
}

void		SocketDependencies::setFd(const int &fd)
{
	this->fd = fd;
}
