#include"FileDepandenc.hpp"


FileDepandenc::FileDepandenc(/* args */)
{
}

FileDepandenc::~FileDepandenc()
{
}

pollfd FileDepandenc::getFd() const
{
	return fd;
}
std::string FileDepandenc::getRequist() const 
{
	return requist;
}
int FileDepandenc::getStatus() const
{
	return status;
}

void FileDepandenc::setFd(pollfd fd)
{
	this->fd = fd;
}
void FileDepandenc::setRequist(std::string requist) 
{
	this->requist = requist;
}
void FileDepandenc::setStatus(int status)
{
	this->status = status;
}