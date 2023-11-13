#ifndef POLL_HPP
#define POLL_HPP

#include <poll.h>
#include <vector>

class   Poll
{
    std::vector<struct pollfd> fds;
    public :
        Poll( void );
        Poll(const std::vector<struct pollfd>& _fds);
        Poll(const Poll& copy);
        ~Poll( void );
        Poll&   operator=(const Poll& target);
        size_t  fdsSize( void );
        int     getReadyFd(int fd);
        int	    waitingRequest( void );
        void    push_fd(int fd);
        void    erase_fd(int fd);

};

#endif