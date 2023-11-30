#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>

class Response
{
private:
    std::string response;
public:
    Response( void );
    ~Response( void );

    void    setResponseLine(std::string version, std::string statusCode, std::string msg);
    void    setHeader(std::string key, std::string value);
    void    setEndHeader();
    void    setBody(std::string content);

    std::string getResponse() const;
    void clear()
    {
        response.clear();
    }
};





#endif