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
            this->URI = reqLine[1];
        if (!reqLine[2].empty())
            this->httpVersion = reqLine[2];
    }
}

std::string RequestLine::getMethode() const
{
    return (this->methode);
}
std::string RequestLine::uri() const
{
    return (this->URI);
}
std::string RequestLine::getHttpVersion() const
{
    return (this->httpVersion);
}

std::string RequestLine::pathname()
{
    std::string pathname;
    size_t pos = 0;
    if ((pos = this->URI.find("?")) != std::string::npos)
    {
        pathname = this->URI.substr(0, pos);
        return (pathname);
    }
    return (this->URI);
}
std::string RequestLine::query()
{
    std::string query;
    size_t pos = 0;
    if ((pos = this->URI.find("?")) != std::string::npos)
    {
        query = this->URI.substr(pos + 1, this->URI.length());
    }
    return (query);
}