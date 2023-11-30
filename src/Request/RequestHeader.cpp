
#include "RequestHeader.hpp" 

RequestHeader::RequestHeader( void )
{
}

RequestHeader::~RequestHeader( void )
{
}

void    RequestHeader::set(std::pair<std::string, std::string> keyValue)
{
    this->requestHeader.insert(keyValue);
}
maps RequestHeader::getRequestHeader( void ) const
{
    return (requestHeader);
}
const std::string &RequestHeader::header(const std::string &key)
{
    return(this->requestHeader[key]);
}

