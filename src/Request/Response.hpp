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


typedef map<string, string> maps;
string readF(const string& fileName);


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
	string message;
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
	void	setMessage(const string &message);
	void	setMimeType(const map<string, string> &mimeTypes);
	void	setRedirection(const string& redirection);
	void	setHeader(const string &key, const string &value);
	void	setBody(const string &body);
	void	setResponse(const string& Response);
	void    setStatusMessage();
	void	setFileToServe(const string &path);

	const string&	getFileToServe() const;
	const string&	getMessage() const;
	const string&	getResponse() const;
	const string&	getRedirection() const;
	const string&	getBody() const;
	string			getMimeType( const string &key ) const;
	string			getStatusMessage( int status );
	const map<int, string>&	getStatusMessage() const;

	void    makeResponse();

	int isDirectory(const string& path);
	int isFile(const string& path);

	void isFormed();
	void isMatched();
	void isRedirected();
	void isMethodAllowed();
	void whichMethod();

	void GetMethod(const string& pathToServe);
	void PostMethod(const string& pathToServe);
	void DeleteMethod(const string& pathToServe);
	void redirection(int code, const string& path);

	void deleteAll (const string& path);

	string getErrorFile(int statusCode) const;
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