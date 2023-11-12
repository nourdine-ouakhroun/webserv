#include "Poll.hpp"

Poll::Poll( void )
{

}

Poll::Poll(const std::vector<struct pollfd>& _fds) : fds(_fds)
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
        fds = target.fds;
    return (*this);
}

int    Poll::getReadyFd(int idx)
{
    if (fds[(std::vector<int>::size_type)idx].revents & POLLIN \
        || fds[(std::vector<int>::size_type)idx].revents & POLLOUT)
        return (fds[(std::vector<int>::size_type)idx].fd);
    return (-1);
}

size_t  Poll::fdsSize( void )
{
    return (fds.size());
}

int		Poll::waitingRequest( void )
{
	return (poll(&fds[0], (nfds_t)fds.size(), 5000));
}

void    Poll::push_fd(int fd)
{
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN | POLLOUT;
    fds.push_back(pfd);
}

void    Poll::erase_fd(int fd)
{
    std::vector<struct pollfd>::iterator begin = fds.begin();
    while (begin < fds.end())
    {
        if (begin->fd == fd)
        {
            fds.erase(begin);
            return ;
        }
        begin++;
    }
}