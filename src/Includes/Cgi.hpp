#ifndef CGI_HPP
#define CGI_HPP
#include<iostream>
#include<map>
#include "ServerData.hpp"
#include <unistd.h>
class   Cgi{
    private :
        std::map<String, String> MultCGI;
        std::string PathScript;
        Cgi();

    public :
        Cgi( std::string ,std::map<String, String>);
        void setCgi( std::string key , std::string valeu);
        ~Cgi();
        std::string HandelScript(std::vector<String>);

};


#endif