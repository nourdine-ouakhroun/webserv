#ifndef CGI_HPP
#define CGI_HPP
#include<iostream>
#include "ServerData.hpp"
#include <unistd.h>
class   Cgi{
    private :
        std::string PathScript;
        Cgi();

    public :
        Cgi( std::string );
        ~Cgi();

        std::string HandelScript();
};


#endif