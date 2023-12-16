#include"FileDependency.hpp"


FileDependency::FileDependency(/* args */)
{
    contenlenght = 0;
    lenght = -1;
	method = -1;
	status = 0;
}

FileDependency::~FileDependency()
{
}
// void FileDependency::eraseRequist(size_t _n, size_t linght)
// {
// 	this->requist.erase(_n, linght);
// }

// const std::string &FileDependency::getRequist() const 
// {
// 	return requist;
// }
// const int &FileDependency::getStatus() const
// {
// 	return status;
// }
const int &			FileDependency::getMethod() const
{
	return this->method;
}
void 			FileDependency::setMethod(const int & method)
{
	this->method = method;
}
void			FileDependency::setFileName(const std::string &filename)
{
	this->filename = filename;
}
const std::string&	FileDependency::getFileName() const
{
	return filename;
}
const std::string & FileDependency::getRequist() const
{
	return this->request;
}

// void FileDependency::setRequist(const std::string& request,const size_t& lenght) 
// {
// 	this->request.append(request, this->request.size(), lenght);
// }
void FileDependency::setRequist(const std::string& request) 
{
	this->request.append(request);
}
void FileDependency::setFdPoll(const int &fd, const short & events)
{
	this->fdpoll.fd = fd;
	this->fdpoll.events = events;
}
void FileDependency::setFdPoll(const short & events)
{
	this->fdpoll.events = events;
}
void FileDependency::setFdPoll(const pollfd &poll_struct)
{
	this->fdpoll = poll_struct;
}

const pollfd &FileDependency::getFdPoll() const
{
    return this->fdpoll;
}

const std::string &FileDependency::getBoundary() const
{
	return this->boundary;
}
const ssize_t& FileDependency::getContenlenght() const
{
	return this->contenlenght;
}
const ssize_t & FileDependency::getLenght() const
{
	return this->lenght;
}
void	FileDependency::appendLenght(const size_t & lenght)
{
	this->lenght += (ssize_t)lenght;

}
void FileDependency::setLenght(const size_t & lenght)
{
	this->lenght = (ssize_t)lenght;
}
void FileDependency::setContenlenght(const ssize_t & contenlenght)
{
	this->contenlenght = contenlenght;
}
void FileDependency::setBoundary(const std::string &	boundary)
{
	this->boundary = boundary;
}

const int	&FileDependency::getFd() const
{
	return this->fd;
}

void		FileDependency::setFd(const int &fd)
{
	this->fd = fd;
}
