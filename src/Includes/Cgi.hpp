#ifndef CGI_HPP
#define CGI_HPP
#include<iostream>
#include<map>
#include "ServerData.hpp"
#include <unistd.h>
class   Cgi{
    private :
        map<String, String> MultCGI;
        string PathScript;
        Cgi();

    public :
        Cgi( string ,map<String, String>);
        void setCgi( string key , string valeu);
        ~Cgi();
        string HandelScript(vector<String>);

};


#endif