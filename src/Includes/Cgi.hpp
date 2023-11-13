#ifndef CGI_HPP
#define CGI_HPP
#include<iostream>
#include <unistd.h>
class Cgi{
    private :
        std::string PathScript;
        Cgi(); 
    public :
        Cgi(std::string);
        std::string HandelScript();
        ~Cgi();
};


#endif