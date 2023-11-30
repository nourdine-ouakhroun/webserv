#ifndef REQUESTHEADER_HPP
#define REQUESTHEADER_HPP

#include <iostream>
#include <vector>
#include <map>

typedef std::map<std::string, std::string> maps;

class RequestHeader
{
private:
    maps    requestHeader;
public:
    RequestHeader( void );
    ~RequestHeader( void );

    void set(std::pair<std::string, std::string> keyValue);
    maps getRequestHeader( void ) const;
    const std::string &header(const std::string &key);

};




#endif