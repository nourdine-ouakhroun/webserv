#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "String.hpp"
#include <map>

class   Request
{
    std::map<String, String> header;
    public :
        Request( void );
        Request(const String& _request);
        Request(const Request& copy);
        ~Request( void );
        Request&    operator=(const Request& target);

};

#endif