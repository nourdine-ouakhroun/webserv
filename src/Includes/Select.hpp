#ifndef SELECT_HPP
#define SELECT_HPP

#include <sys/select.h>
#include <iostream>

class Select
{
    fd_set  set;
    int     nfds;

    public :
        Select( void );
        Select(int fd);
        Select(const Select& copy);
        ~Select( void );
        Select& operator=(const Select& target);
        void    setNumberOfFds(int port);
        fd_set  getSet( void );
        int     getNumberOfFds( void ) const;
        void    fd_zero( void );
        void    fd_set(int fd);
        bool    fd_isset(int fd);
        void    fd_clear(int fd);
        int		waitingRequest( void );
        void    printFds( void );
};

#endif