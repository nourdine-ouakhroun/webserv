#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include <unistd.h>

#include "Request.hpp"
#include "webserver.h"
#include "Servers.hpp"

typedef std::map<std::string, std::string> maps;
int isDirectory(const std::string& path);
string readF(const std::string& fileName);


#define VERSION "HTTP/1.1 "
#define ENDLINE "\r\n"

// class Request;
class Response
{
private:
    const Request&          request;
    const ServerPattern&    server;
    GeneralPattern         location;


	int         statusCode;
    std::string msg;

    maps    header;
    string  body;
    string  response;

    map<int, string> errorPage;

    string fileToServe;

    // Request     request;

    maps mimeType;
    string redirection;



public:
    Response( const Request& req, const ServerPattern& server );
    ~Response( void );

    const map<int, string>& getErrorPage() const;


    void    setErrorPage();
    string  getErrorPage( int status );


    void setStatusCode(int statusCode);
    void setMsg(const std::string &msg);

    void setHeader(const std::string &key, const std::string &value);
    void setBody(const std::string &body);
    const string& getBody() const;

    void setFileToServe(const std::string &fileName);



    // void setRequest(const Request &request);

    // const	std::string &getVersion( void ) const;
    int		getStatusCode( void ) const;
    const	std::string &getMsg( void ) const ;
    const   std::string &getResponse( void ) const;
    // const Request &getRequest(void) const;


    void    makeResponse( void );

	void   setMimeType(const map<string, string> &mimeTypes);
    string getMimeType( const std::string &key ) const;




    void isFormed();
    void isMatched();
    void isRedirected();
    void isMethodAllowed();
    void whichMethod();

    void GetMethod();
    void PostMethod();
    void DeleteMethod();


    // geters of cobnfigFile
    // std::string checkServer( void );
    // std::string getFullPath();
    std::string getErrorFile(int statusCode) const;
    string      getRoot() const;
    string      isFound(const string &path) const;
    bool        isCgi();
    string      isUpload();

    string      getCgiFile();

    string      runScript(vector<String> args, string fileName);

    const string&   getRedirection() const;
    void            setRedirection(const string& redirection);
};

#endif