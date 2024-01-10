#include "Request.hpp"

Request::Request( void )
{
}

Request::Request(const String& _request)
{
    
}
Request::Request(const Request& copy)
{
    *this = copy;
}
Request::~Request( void )
{
}

Request& Request::operator=(const Request& target)
{
    if (this != &target)
        header = target.header;
    return (*this);
}