#include "Select.hpp"

Select::Select( void )
{
    fd_zero();
    nfds = 0;
}

Select::Select(int fd)
{
    fd_zero();
    fd_set(fd);
    nfds = fd;
}

Select::Select(const Select& copy)
{
    *this = copy;
}

Select::~Select( void )
{

}

Select& Select::operator=(const Select& target)
{
    if (this != &target)
    {
        set = target.set;
        nfds = target.nfds;
    }
    return (*this);
}


void    Select::setNumberOfFds(int nfd)
{
    nfds = nfd;
}

fd_set  Select::getSet( void )
{
    return (set);
}

int     Select::getNumberOfFds( void ) const
{
    return (nfds);
}

void    Select::fd_zero( void )
{
    FD_ZERO(&set);
}

void    Select::fd_set(int fd)
{
    FD_SET(fd, &set);
    nfds = fd;
}

bool     Select::fd_isset(int fd)
{
    return (FD_ISSET(fd, &set) ? true : false);
}

void    Select::fd_clear(int fd)
{
    FD_CLR(fd, &set);
}

int		Select::waitingRequest( void )
{
    timeval time;
    time.tv_sec = 2;
    time.tv_usec = 500;
	return (select(nfds + 1, &set, NULL, NULL, &time));
}


#include <stdio.h>
void printBits(unsigned int num){
    int size = sizeof(unsigned int);
    int maxPow = 1 << (size * 8 - 1);
    printf("MAX POW : %u\n",maxPow);
    int i=0;
    for(;i < size*8;++i)
    {
        printf("%u ",num&(unsigned int)maxPow ? 1 : 0);
        num = num<<1;
    }
}

void    Select::printFds( void )
{
    for (int i = 0; i <= 2; i++)
    {
        printBits((unsigned int)set.fds_bits[i]);
        cout << endl;

    }
}