#include"FileDepandency.hpp"


FileDepandenc::FileDepandenc(/* args */)
{
    contenlenght = 0;
    lenght = 0;
    status = false;
	int 		method = -1;
}

FileDepandenc::~FileDepandenc()
{
}
// void FileDepandenc::eraseRequist(size_t _n, size_t linght)
// {
// 	this->requist.erase(_n, linght);
// }

// const std::string &FileDepandenc::getRequist() const 
// {
// 	return requist;
// }
// const int &FileDepandenc::getStatus() const
// {
// 	return status;
// }

void FileDepandenc::setRequist(std::string requist, size_t lenght) 
{
	this->requist.append(re);
}
void FileDepandenc::setFdPoll(int fd, short events)
{
	this->fdpoll.fd = fd;
	this->fdpoll.events = events;
}
void FileDepandenc::setFdPoll(pollfd &poll_struct)
{
	this->fdpoll = poll_struct;
}
pollfd &FileDepandenc::getFdPoll() const 
{
    this->fdpoll;
}