#ifndef CGI_HPP
#define CGI_HPP
#include<iostream>
#include<map>
#include "ServerData.hpp"
#include <unistd.h>
class   Cgi{
    private :
        std::map<std::string, std::string> MultCGI;
        std::string PathScript;
        Cgi();

    public :
        Cgi( std::string );
        ~Cgi();
        std::string HandelScript(std::vector<String>);

};


#endif