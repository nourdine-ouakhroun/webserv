#ifndef POLL_HPP
#define POLL_HPP

#include <sys/poll.h>
#include <netinet/in.h>
#include <vector>
#include <cstddef>
#include <climits>

using namespace std;

class   Poll
{
    public :
        vector<struct pollfd> fds;
        vector<struct sockaddr_in> clientInfo;
        Poll( void );
        Poll(const vector<struct pollfd>& _fds);
        Poll(const Poll& copy);
        ~Poll( void );
        Poll&   operator=(const Poll& target);
        unsigned int fdsSize( void );
        int     getReadyFd(int fd);
        int	    waitingRequest( void );
        void    push_fd(int fd, struct sockaddr_in _clientInfo);
        void    erase_fd(int fd);
        struct sockaddr_in  getClientInfo(int fd);

};

#endif
