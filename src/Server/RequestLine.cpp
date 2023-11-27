#include "RequestLine.hpp"

RequestLine::RequestLine( void )
{
}

RequestLine::~RequestLine()
{
}

void RequestLine::set(std::vector<std::string> reqLine)
{
    if (!reqLine.empty())
    {
        if (!reqLine[0].empty())
            this->methode = reqLine[0];
        if (!reqLine[1].empty())
            this->query = reqLine[1];
        if (!reqLine[2].empty())
            this->httpVersion = reqLine[2];
    }
}

std::string RequestLine::getMethode() const
{
    return (this->methode);
}
std::string RequestLine::getQuery() const
{
    return (this->query);
}
std::string RequestLine::getHttpVersion() const
{
    return (this->httpVersion);
}