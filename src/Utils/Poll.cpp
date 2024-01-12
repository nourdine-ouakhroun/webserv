#include "Poll.hpp"

Poll::Poll( void )
{

}

Poll::Poll(const vector<struct pollfd>& _fds) : fds(_fds)
{
}

Poll::Poll(const Poll& copy)
{
    *this = copy;
}

Poll::~Poll( void )
{
    fds.clear();
}

Poll&   Poll::operator=(const Poll& target)
{
    if (this != &target)
    {
        fds = target.fds;
        clientInfo = target.clientInfo;
    }
    return (*this);
}

int    Poll::getReadyFd(int idx)
{
    if (fds[(vector<int>::size_type)idx].revents & POLLIN \
        || fds[(vector<int>::size_type)idx].revents & POLLOUT)
        return (fds[(vector<int>::size_type)idx].fd);
    return (-1);
}

unsigned int Poll::fdsSize( void )
{
    if (fds.size() > (size_t)UINT_MAX)
        return (UINT_MAX);
    return ((unsigned int)fds.size());
}

int		Poll::waitingRequest( void )
{
	return (poll(&fds[0], (nfds_t)fds.size(), 5000));
}

void    Poll::push_fd(int fd, struct sockaddr_in _clientInfo)
{
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN | POLLOUT;
    fds.push_back(pfd);
    clientInfo.push_back(_clientInfo);
}

void    Poll::erase_fd(int fd)
{
    for (size_t i = 0; i < fds.size(); i++)
    {
        if (fds.at(i).fd == fd)
        {
            fds.erase(fds.begin() + (long)i);
            clientInfo.erase(clientInfo.begin() + (long)i);
            return ;
        }
    }
}

struct sockaddr_in  Poll::getClientInfo(int fd)
{
    for (size_t i = 0; i < fds.size(); i++)
        if (fds.at(i).fd == fd)
            return (clientInfo.at(i));
    throw (exception());
}
