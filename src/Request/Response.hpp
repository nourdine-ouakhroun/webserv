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
string readF(const std::string& fileName);


#define VERSION "HTTP/1.1 "
#define ENDLINE "\r\n"

// class Request;
class Response
{
private:
	const Request&          request;
	const ServerPattern&    server;
	GeneralPattern        	location;


	int         statusCode;
	std::string message;
	maps    	header;
	string		body;
	string		response;


	map<int, string>		statusMessage;
	map<string, string>		mimeType;

	string					fileToServe;
	string					_redirection;




public:
string pathToServe;
	Response( const Request& req, const ServerPattern& server );
	~Response( void );
	void	setStatusCode(int statusCode);
	void	setMessage(const std::string &message);
	void	setMimeType(const map<string, string> &mimeTypes);
	void	setRedirection(const string& redirection);
	void	setHeader(const std::string &key, const std::string &value);
	void	setBody(const std::string &body);
	void	setResponse(const string& Response);
	void    setStatusMessage();
	void	setFileToServe(const std::string &path);

	const string&	getFileToServe() const;
	const string&	getMessage() const;
	const string&	getResponse() const;
	const string&	getRedirection() const;
	const string&	getBody() const;
	string			getMimeType( const std::string &key ) const;
	string			getStatusMessage( int status );
	const map<int, string>&	getStatusMessage() const;

	void    makeResponse();

	int isDirectory(const std::string& path);
	int isFile(const std::string& path);

	void isFormed();
	void isMatched();
	void isRedirected();
	void isMethodAllowed();
	void whichMethod();

	void GetMethod();
	void PostMethod();
	void DeleteMethod();
	void redirection(int code, const string& path);

	void deleteAll (const string& path);

	std::string getErrorFile(int statusCode) const;
	string      getRoot() const;
	string      getAlias() const;
	string      isFound(const string &path) const;
	bool        isCgi();
	string      isUpload();
	string      getCgiFile();
	string      getLocationPath();

	string      runScript(vector<String> args, string fileName);
};

#endif