
#ifndef REQUESTLINE_HPP
#define REQUESTLINE_HPP

#include <iostream>
#include <vector>
#include <map>

typedef std::map<std::string, std::string> maps;


class RequestLine
{
private:
    std::string methode;
    std::string URI;
    std::string httpVersion;
    // std::string pathname;
    // std::string query;

public:
    RequestLine( void );
    ~RequestLine();

    // set attribute
    void set(std::vector<std::string>);

    // get attribute
    std::string getMethode() const;
    std::string uri() const;
    std::string getHttpVersion() const;
    std::string pathname();
    std::string query();
};




#endif