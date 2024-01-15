#include"Socket.hpp"


Socket::Socket(/* args */)
{
    content_lenght = 0;
	is_chuncked = 0;
    // lenght = -1;
	method = -1;
	// status = 0;
	// rest = 0;
	hex_valeu = 0;
}
// Socket::Socket(const Socket&	socket)
// {
// 	*this = socket;
// }
// Socket &Socket::operator=(const Socket&	socket)
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

Socket::~Socket()
{
}
// void Socket::eraseRequest(size_t _n, size_t linght)
// {
// 	this->Request.erase(_n, linght);
// }

// const string &Socket::getHeader() const 
// {
// 	return Request;
// }
// const int &Socket::getStatus() const
// {
// 	return status;
// }
const int &			Socket::getMethod() const
{
	return this->method;
}
void 			Socket::setMethod(const int & method)
{
	this->method = method;
}
// void			Socket::setFileName(const string &filename)
// {
// 	this->filename = filename;
// }
// const string&	Socket::getFileName() const
// {
// 	return filename;
// }
const string & Socket::getHeader() const
{
	return this->header;
}
const string & Socket::getBody() const
{
	return this->body;
}

// void Socket::setHeader(const string& request,const size_t& lenght) 
// {
// 	this->request.append(request, this->request.size(), lenght);
// }
void Socket::setHeader(const string& header) 
{
	this->header.append(header);
}
void Socket::setBody(const string& body) 
{
	this->body.append(body);
}
void Socket::setFdPoll(const int &fd, const short & events)
{
	this->fdpoll.fd = fd;
	this->fdpoll.events = events;
}
void Socket::setFdPoll(const short & events)
{
	this->fdpoll.events = events;
}
void Socket::setFdPoll(const pollfd &poll_struct)
{
	this->fdpoll = poll_struct;
}

const pollfd &Socket::getFdPoll() const
{
    return this->fdpoll;
}

const string &Socket::getBoundary() const
{
	return this->boundary;
}
const ssize_t& Socket::getContenlenght() const
{
	return this->content_lenght;
}
// const ssize_t & Socket::getLenght() const
// {
// 	return this->lenght;
// }
// void	Socket::appendLenght(const size_t & lenght)
// {
// 	this->lenght += (ssize_t)lenght;

// }
// void Socket::setLenght(const size_t & lenght)
// {
// 	this->lenght = (ssize_t)lenght;
// }
void Socket::setContenlenght(const ssize_t & contenlenght)
{
	this->content_lenght = contenlenght;
}
void Socket::setBoundary(const string &	boundary)
{
	this->boundary = boundary;
}

// const int	&Socket::getFd() const
// {
// 	return this->fd;
// }

// void		Socket::setFd(const int &fd)
// {
// 	this->fd = fd;
// }
