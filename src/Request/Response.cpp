#include "Response.hpp"


string Response::runScript(vector<String> args, string fileName)
{
	string		body;
	int			forkValue(0);
	int			fd[2];
	int			exitStatus;
	string		extention;
	size_t		pos;


	char	*argv[] = {
		strdup(args[0].c_str()),
		strdup(fileName.c_str()),
		NULL
	};
	char	*envp[] = {
		
		strdup(("HTTP_CONNECTION=" + request.header("Connection")).c_str()),
		strdup(("HTTP_HOST=" + request.header("Host")).c_str()),
		strdup(("CONTENT_LENGTH=" + request.header("Content-Length")).c_str()),
		strdup(("CONTENT_TYPE=" + request.header("Content-Type")).c_str()),
		strdup(("HTTP_ACCEPT=" + request.header("Accept")).c_str()),
		strdup(("HTTP_USER_AGENT=" + request.header("User-Agent")).c_str()),
		strdup(("PATH_INFO=" + request.getPath()).c_str()),
		strdup(("SCRIPT_NAME=" + fileName).c_str()),
		strdup(("QUERY_STRING=" + request.getQuery()).c_str()),
		strdup(("REQUEST_METHOD=" + request.getMethod()).c_str()),
		NULL
	};

	

	if ((pos = fileName.rfind(".")) != string::npos)
		extention = fileName.substr(pos, fileName.size() - pos);
	if (!extention.empty() && args.size() == 2) {
		if (args[1] == extention) {
			int valuePipe = pipe(fd);
			if (valuePipe < 0)
				exit(0);
			forkValue = fork();
			if (forkValue == 0)
			{

				dup2(fd[1], STDOUT_FILENO);
				dup2(fd[0], STDIN_FILENO);
				if (execve(argv[0], argv, envp) < 0)
				{
					cout << "execve failed" << endl;
					exit(0);
				}
			}
			else{
				write(fd[1], request.getBody().c_str(), request.getBody().size());
				waitpid(forkValue, &exitStatus, 0);
				if (WIFEXITED(exitStatus)) {
					if (WEXITSTATUS(exitStatus) != 0)
					{
						return "<h1> RUN ERROR </h1>";
					}
				}
				char res[200];
				bzero(res, 200);
				ssize_t bytes = 0;
				while((bytes = read(fd[0], res, 199)) != 0)
				{
					body.append(res);
					bzero(res, 200);
					if (bytes < 199)
						break;
				}
				close(fd[0]);
				close(fd[1]);
			}
		}

	}
	return body;
}


Response::Response( const Request& req, const ServerPattern& server ) : request(req), server(server)
{
	setStatusCode(200);
	setMsg("OK");
	setHeader("Server", "Nginx-v2");
	setErrorPage();
	(void)this->request;
	(void)this->server;
}

Response::~Response(void) {
}

void Response::setErrorPage()
{
	errorPage[200] = "OK";
	errorPage[201] = "Created";
	errorPage[204] = "No Content";

	errorPage[301] = "Move Permanently";

	errorPage[404] = "Not Found";
	errorPage[400] = "Bad Request";
	errorPage[403] = "Forbidden";

	errorPage[414] = "Request-Uri Too Longe";
	errorPage[413] = "Request Entity Too Longe";
	errorPage[405] = "Method Not Allowed";

	errorPage[501] = "Not Implamented";
	errorPage[500] = "Internal Server Error";
}
std::string Response::getErrorPage( int status ) {
	return (errorPage[status]);
}


void Response::setFileToServe(const std::string &fileName)
{
	fileToServe = fileName;
}

void Response::setStatusCode( int statusCode )
{
    this->statusCode = statusCode;
}
void Response::setMsg( const std::string &msg )
{
    this->msg = msg;
}
void Response::setHeader(const std::string &key, const std::string &value)
{
    header[key] = value;
}
void Response::setBody(const std::string &body)
{
    this->body = body;
}


// geters
int Response::getStatusCode( void ) const
{
    return (this->statusCode);
}
const std::string &Response::getMsg( void ) const
{
    return (this->msg);
}
const std::string &Response::getResponse( void ) const
{
    return (this->response);
}
const string& Response::getBody() const {
	return (body);
}


void Response::makeResponse( void ) {
	response += VERSION + std::to_string(getStatusCode()) + " " + getMsg() + ENDLINE;
	if (!header.empty()) {
		for( maps::iterator it = header.begin(); it != header.end(); it++) {
			response += it->first + ": " + it->second + ENDLINE; 
		}
	}
	response += ENDLINE;
	if (!body.empty()) {
		response += body + "\n";
	}
}


std::string Response::getMimeType( const std::string &key) const
{
	try {
		std::string ret = mimeType.at(key);
		return (ret);
	}
	catch(...){
		return ("text/html");
	}
}


void Response::setMimeType(const map<string, string>& mimeType)
{
    this->mimeType["csv"] = "text/csv";
    this->mimeType["doc"] = "application/msword";
    this->mimeType["css"] = "text/css";
    this->mimeType["gif"] = "image/gif";
    this->mimeType["html"] = "text/html";
    this->mimeType["ico"] = "image/vnd.microsoft.icon";
    this->mimeType["js"] = "text/javascript";
    this->mimeType["mp3"] = "audio/mpeg";
    this->mimeType["mp4"] = "video/mp4";
    this->mimeType["mpeg"] = "video/mpeg";
    this->mimeType["jpg"] = "image/jpeg";
    this->mimeType["png"] = "image/png";
    this->mimeType["woff"] = "font/woff";
    this->mimeType["woff2"] = "font/woff2";
    this->mimeType["ttf"] = "font/ttf";

	if (mimeType.size()) {
		this->mimeType = mimeType;
	}
}












bool isAllowdChar(const string& uri) {
	string alloweChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
	for (size_t i = 0; i < uri.size(); i++)
	{
		if (find(alloweChar.begin(), alloweChar.end(), uri[i]) == alloweChar.end() || (uri[i] == '%' && isdigit(uri[i + 1])) )
			return (false);
	}
	return (true);
}
void Response::isFormed()
{
	// cout << "|"<< request.header("Transfer-Encoding") << "|" << endl;
	if (!request.header("Transfer-Encoding").empty() && request.header("Transfer-Encoding") != "chunked")
		// throw make_pair(500, "<h1 style=\"text-align: center;\">404 Page not found.</h1>");
		throw 500;
	else if ((request.header("Transfer-Encoding").empty() && request.header("Content-Length").empty() && request.getMethod() == "POST") || !isAllowdChar(request.getPath()))
		throw 400;
	else if (request.getPath().length() > 2048)
		throw 414;
	// else if (!server.getData("client_max_body_size").empty()
	// && std::atoi(request.header("Content-Length").c_str()) > (int)(std::strtod(server.getData("client_max_body_size")[0].getValue().c_str())))
	else if ((double)request.getBody().size() > std::strtod(server.getData("client_max_body_size")[0].getValue().c_str(), NULL))
		throw 413;
}
void Response::isMatched() {

	std::string path = request.getPath();


	vector<String> pathss;
	ServerPattern::getAllLocationPath(server.getLocations(), pathss);
	vector<String> newPaths;
	String test(path);
	test.rightTrim("/");

	for (size_t i = 0; i < pathss.size(); i++)
	{
		if (!strncmp(pathss[i].c_str(), test.c_str(), pathss[i].size()) && pathss[i].size() == test.size())
		{
			newPaths.clear();
			newPaths.push_back(pathss[i]);
			break;
		}
		if (!strncmp(pathss[i].c_str(), test.c_str(), pathss[i].size()))
			newPaths.push_back(pathss[i]);
			// localhost/web/app1/hello
			// /web
			// /web/app1
	}
	vector<String>::iterator it = max_element(newPaths.begin(), newPaths.end());
	if (it == newPaths.end())
		location = server;
	else
		location = server.getLocationByPath(server.getLocations(), *it);
	// if (location.getPath().empty()) {
	// 	location = server;
	// 	// throw 404;
	// }
}
void Response::isRedirected() {
	if (!location.empty()) {
		vector<Data> redirection = location.getData("return");
		if (redirection.size()) {
			this->redirection = redirection[0].getValue();
			throw 301;
		}
	}
}
void Response::isMethodAllowed() {
	vector<Data> method = location.getData("method");
	if (!method.empty()) {
		vector<string> methods = split(method.front().getValue(), " ");
		if (find(methods.begin(), methods.end(), request.getMethod()) == methods.end())
			throw 405;
	}
}
void Response::whichMethod() {
	if (request.getMethod() == "GET") {
		GetMethod();
		// handle Get method
	}
	else if (request.getMethod() == "POST") {
		// handle Post method
		PostMethod();
	}
	else if (request.getMethod() == "DELETE") {
		// handle Delete method
		DeleteMethod();
	}
}

// geters configFile
// std::string Response::getFullPath()
// {
// 	bool isFound = false;
// 	std::string path = request.getPath();
// 	std::string root;
// 	std::string fullPath;

// 	vector<String> pathss;
// 	ServerPattern::getAllLocationPath(server.getLocations(), pathss);
// 	vector<String> newPaths;
// 	String test(path);
// 	test.rightTrim("/");
// 	for (size_t i = 0; i < pathss.size(); i++)
// 	{
// 		if (!strncmp(pathss[i].c_str(), test.c_str(), pathss[i].size()) && pathss[i].size() == test.size())
// 		{
// 			newPaths.clear();
// 			newPaths.push_back(pathss[i]);
// 			break;
// 		}
// 		if (!strncmp(pathss[i].c_str(), test.c_str(), pathss[i].size()))
// 			newPaths.push_back(pathss[i]);
// 	}
// 	vector<String>::iterator it = max_element(newPaths.begin(), newPaths.end());
// 	if (it == newPaths.end())
// 		throw 404;
// 	location = server.getLocationByPath(server.getLocations(), newPaths[0]);
// 	cout << "hello => " << location.getPath().empty() << endl;
// 	if (!location.getPath().empty())
// 	{
// 		std::vector<Data> vRoot = location.getData("root");
// 		if (!vRoot.empty())
// 		{
// 			root = vRoot[0].getValue();
// 			if (isDirectory(root + request.getPath()) == 1 && (root + request.getPath()).back() != '/')
// 				throw 301;
// 		}
// 		if (!(isDirectory(root + path) == 1))
// 		{
// 			return (root + path);
// 		}
// 		std::vector<Data> vIndex = location.getData("index");
// 		if (!vIndex.empty())
// 		{
// 			for (size_t i = 0; (i < vIndex.size() && !isFound); i++)
// 			{
// 				std::vector<std::string> filename = split(vIndex[i].getValue(), " ");
// 				for (size_t j = 0; (j < filename.size() && !isFound); j++)
// 				{
// 					std::string fileExist = root + path + filename[j];
// 					if (access(fileExist.c_str(), O_RDONLY) != -1)
// 					{
// 						fullPath = fileExist;
// 						isFound = true;
// 					}
// 				}
// 			}
// 		}
// 	}
// 	return (fullPath);
// }
std::string Response::getErrorFile(int statusCode) const
{
	vector<Data>  errorPage = location.getData("error_page");

	for (size_t i = 0; i < errorPage.size(); i++) {
		vector<string> value = split(errorPage[i].getValue(), " ");
		for (size_t j = 0; j < value.size() - 1; j++) {
			if (statusCode == atoi(value[j].c_str()))
				return (value[value.size() - 1]);
		}
	}
	return ("");
}
string Response::getRoot() const{
	if (!location.empty())
		return (location.getData("root")[0].getValue());
	return ("");
}
string Response::isFound( const string& path ) const
{
	std::vector<Data> vIndex = location.getData("index");
	if (!vIndex.empty())
	{
		for (size_t i = 0; i < vIndex.size(); i++)
		{
			std::vector<std::string> filename = split(vIndex[i].getValue(), " ");
			for (size_t j = 0; j < filename.size(); j++)
			{
				if (access((path + filename[j]).c_str(), O_RDONLY) != -1)
					return (filename[j]);
			}
		}
	}
	return ("");
}
bool Response::isCgi() {
	vector<Data> cgiFiles = location.getData("cgi");
	if (cgiFiles.empty())
		return (false);
	return (true);
}
string Response::getCgiFile()
{
	vector<Data> cgiFiles = location.getData("cgi");
	if ( !cgiFiles.empty() )
		return cgiFiles[0].getValue();
	return "";
}

string Response::isUpload() {
	vector<Data> uploadFile = location.getData("upload_dir");
	if ( !uploadFile.empty())
		return (uploadFile[0].getValue());
	else
		return ("");
}











void Response::GetMethod() {
	std::string path = getRoot() + request.getPath();
	if (access(path.c_str(), O_RDONLY))
		throw 404;
	if (isDirectory(path)) {
		// is Directory
		if (path[path.size() - 1] != '/') {
			redirection = request.getPath() + "/";
			throw 301;
		}
		// if (access((root + path + index).c_str(), O_RDONLY) == -1) { //
		string index = isFound(path);
		if (!index.empty()) {
			// success have a index
			if (access((path + index).c_str(),  O_RDONLY) != -1)
				throw 200;
			else if (isCgi()) // if location have cgi
				// run cgi here
				body = runScript(location.getData("cgi")[0].getValue().split(' ') , path + index);
		}
		else
		{
			if (!this->location.getData("autoindex").empty() && this->location.getData("autoindex")[0].getValue() == "on") {
				// on return autoindex of the directory
				cout << "autoindex work" << endl;
				DIR *directory = opendir(path.c_str());
				if (directory == NULL)
					return ;
				struct dirent *dir;
				body.append("<!DOCTYPE html>")
					.append("<html lang=\"en\">")
					.append("<head>")
					.append("<meta charset=\"UTF-8\">")
					.append("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">")
					.append("<title>Document</title>")
					.append("</head>")
					.append("<body>")
					.append("<h1> autoIndex </h1>");
				while ( (dir = readdir(directory))  != NULL ) {
						body.append("<a href=\"")
            			.append(dir->d_name)
            			.append("\">").append(dir->d_name)
           				.append("</a></br>");
				}
				body.append("</body>");
			}
			else
				throw 403;
			// auto index
			// check if autoindex in configfile is on or off if is off 403 otherwise return autoindex
		}
	}
	else {
		// is File
		size_t pos;
		string extention;
		if ((pos = path.rfind(".")) != string::npos)
			extention = path.substr(pos, path.size() - pos);
		if (isCgi() && !extention.empty() && extention == location.getData("cgi")[0].getValue().split(' ')[1])
			// run cgi here
			body = runScript(location.getData("cgi")[0].getValue().split(' ') , path);
		else
			throw 200;
	}
	throw 200;
}

void Response::PostMethod() {
	string uploadDir = isUpload();
	if (!uploadDir.empty()) {
		cout << "Upload" << endl;

		if (uploadDir.back() != '/')
			uploadDir.append("/");
		vector<pair<string, string> > upload = request.getUploads();

		for (size_t i = 0; i < upload.size(); i++)
		{
			if (!upload[i].first.empty()) {
				ofstream up(uploadDir + upload[i].first);
				up << upload[i].second;
				up.close();
			}
		}
	}
	else {
		cout << "Not Upload" << endl;
		std::string path = getRoot() + request.getPath();

		if (access(path.c_str(), O_RDONLY))
			throw 404;
		if (isDirectory(path)) {
			// is Directory
			if (path[path.size() - 1] != '/') {
				redirection = request.getPath() + "/";
				throw 301;
			}
			// if (access((root + path + index).c_str(), O_RDONLY) == -1) { //
			string index = isFound(path);
			if (!index.empty()) {
				// success have a index
				if (access((path + index).c_str(),  O_RDONLY) != -1)
					throw 200;
				else if (isCgi()) // if location have cgi
					body = runScript(location.getData("cgi")[0].getValue().split(' ') , path + index);
					// run cgi here
			}
			else
				throw 403;
		}
		else {
			// is File
			size_t pos;
			string extention;
			if ((pos = path.rfind(".")) != string::npos)
				extention = path.substr(pos, path.size() - pos);
			cout << isCgi() << endl;
			if (isCgi() && !extention.empty() && extention == location.getData("cgi")[0].getValue().split(' ')[1])
				body = runScript(location.getData("cgi")[0].getValue().split(' ') , path);
				// run cgi here
			else
				throw 200;
		}
		throw 200;
	}
}

void Response::DeleteMethod() {
}


const string& Response::getRedirection() const {
	return (redirection);
}
void Response::setRedirection(const string& redirection)
{
	this->redirection = redirection;
}
const map<int, string>& Response::getErrorPage() const {
	return (errorPage);
}