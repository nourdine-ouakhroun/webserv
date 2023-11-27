
#ifndef REQUESTLINE_HPP
#define REQUESTLINE_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include <vector>
#include <sstream>



#include <arpa/inet.h>
#include <fstream>
#include <ctime>

typedef std::map<std::string, std::string> maps;


class RequestLine
{
private:
    std::string methode;
    std::string query;
    std::string httpVersion;
public:
    RequestLine( void );
    ~RequestLine();

    // set attribute
    void set(std::vector<std::string>);

    // get attribute
    std::string getMethode() const;
    std::string getQuery() const;
    std::string getHttpVersion() const;
};




#endif