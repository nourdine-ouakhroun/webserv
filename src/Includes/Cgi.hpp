#ifndef CGI_HPP
#define CGI_HPP
#include<iostream>
#include<map>
#include "ServerData.hpp"
#include <unistd.h>
#include"../Request/Request.hpp"
class Request;
class   Cgi{
    private :
		vector<string> env;
        string scriptName;
        string body;
        Cgi();

    public :
        Cgi(const string& scriptName,const string& body);
        Cgi(const string& scriptName);
        ~Cgi();
        void setEnvironment(const Request& header, const string &script_name);
        string	HandelScript();

};


#endif