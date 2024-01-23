#include "Response.hpp"
// std::string readF(const std::string &path);
// std::string readFile(const std::string &path)
// {
// 	std::string content;
// 	char buffer[READ_SIZE + 1];

// 	int fd = open(path.c_str(), O_RDONLY);
// 	if (fd < 0)
// 		return (content);

// 	while (1)
// 	{
// 		ssize_t readBytes = read(fd, buffer, READ_SIZE);
// 		if (readBytes <= 0)
// 			break;
// 		buffer[readBytes] = 0;
// 		content.append(buffer, (u_long)readBytes);
// 	}
// 	close(fd);
// 	return (content);
// }

	Response::Response(void) : statusCode(200), msg("OK")
{
	setErrorPage();
	// setMimeType();
}
Response::~Response(void) {
}

void Response::setErrorPage()
{
	errorPage[200] = "OK";
	errorPage[201] = "Created";
	errorPage[204] = "No Content";
	errorPage[404] = "Not Found";
	errorPage[501] = "Not Implamented";
	errorPage[400] = "Bad Request";
	errorPage[414] = "Request-Uri Too Longe";
	errorPage[413] = "Request Entity Too Longe";
	errorPage[301] = "Move Permanently";
	errorPage[303] = "Forbidden";
	errorPage[405] = "Method Not Allowed";
	errorPage[500] = "Internal Server Error";
}
std::string Response::getErrorPage( int status ) {
	return (errorPage[status]);
}


void Response::setResponse(int status)
{
	cout << "CODE = " << status << endl;
	string file = (request->getRoot() + request->getPathname() + request->isFound(request->getRoot() + request->getPathname()));
	cout << "file " << file << endl;
	
	string content;
	setStatusCode(status);
	setMsg(getErrorPage(status));
	// setHeader(getErrorPage(status));

	if (status == 301) {
		setHeader("Location", request->getPathname() + "/");
	}
	else if (status != 200) {
		string errorFile = request->getErrorFile(status);

		if (!errorPage.empty() && access(errorFile.c_str(), O_RDONLY) == -1) {
			content = "<h1>" + getErrorPage(status) + "</h1>";
			setBody(content);
		}
		else {
			content = Request::readFile(errorFile);
			setBody(content);
		}
		setHeader("Content-Type", "text/html");
		setHeader("Content-Length", to_string(content.size()));
	}
	else {
		// success OK
	}
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
    _header[key] = value;
}
void Response::setBody(const std::string &body)
{
    _body = body;
}


// geters
int Response::getStatusCode( void ) const
{
    return (this->statusCode);
}
// const std::string &Response::getVersion( void ) const
// {
//     return(this->version);
// }
const std::string &Response::getMsg( void ) const
{
    return (this->msg);
}
const std::string &Response::getResponse( void ) const
{
    return (this->response);
}
// const Request &Response::getRequest(void) const
// {
// 	return (request);
// }

void Response::makeResponse( void ) {
	response += VERSION + std::to_string(getStatusCode()) + " " + getMsg() + ENDLINE;
	if (!_header.empty()) {
		for( maps::iterator it = _header.begin(); it != _header.end(); it++) {
			response += it->first + ": " + it->second + ENDLINE; 
		}
	}
	response += ENDLINE;
	if (!_body.empty()) {
		response += _body + "\n";
	}
}



//const	std::string &getHeader(const std::string &key, const std::string &value) const
//{
//	return ()
//}
// const std::string &Response::getBody( void ) const
// {
//     return (this->body);
// }





// bool	allowdSpecialchar(char chr)
// {
// 	std::string specialChar = "_:;.,\\/\"'?!(){}[]@<>=-+*#$&`|~^%";
// 	for(size_t i = 0; i < specialChar.length(); i++)
// 	{
// 		if (chr == specialChar[i])
// 			return (1);
// 	}
// 	return (0);
// }


// bool     Response::isAllowed(const std::string &url)
// {
// 	for(size_t i = 0; i < url.length(); i++)
// 	{
// 		if (!isprint(url[i]))
// 			return (0);
// 	}
// 	return (1);
// }

// bool Response::isFormed(Request req)
// {
//     if ((!req.header("Transfer-Encoding").empty() && req.header("Transfer-Encoding") != "chunked") || !this->isAllowed(req.getPathname()))
//     {
//         this->statusCode = 501;
//         this->version = req.getVersion();
//         this->msg = "Not Implemented";
//         return (0);
//     }
//     else if (req.getMethod() == "GET" && req.header("Transfer-Encoding").empty() && req.header("Content-Length").empty())
//     {
//         this->statusCode = 400;
//         this->version = req.getVersion();
//         this->msg = "Bad Request";
//         return (0);
//     }
//     else if (req.getPathname().length() > 2048)
//     {
//         this->statusCode = 414;
//         this->version = req.getVersion();
//         this->msg = "Request-URI To Long";
//         return (0);
//     }
//     else if (!req.header("Content-Length").empty() && std::atoi(req.header("Content-Length").c_str()) > 1024 )
//     {
//         this->statusCode = 413;
//         this->version = req.getVersion();
//         this->msg = "Request Entity To Long";
//         return (0);
//     }
//     return (1);
// }

// bool Response::isMatched( const Request &req, std::vector<ServerPattern> servers )
// {
//     for (size_t server = 0; server < servers.size(); server++)
//     {
//         std::string fullPath;
// 		std::string root;
//         std::vector<Data> data = servers[server].getLocationByPath(servers[server].getLocation(), req.getPathname()).getAllData();
//         for (size_t dataIndex = 0; dataIndex < data.size(); dataIndex++)
//         {
//            if (data[dataIndex].getKey() == "root")
// 				root = data[dataIndex].getValue();
// 			else if (data[dataIndex].getKey() == "alias")
// 				root = data[dataIndex].getValue();
// 			else if (data[dataIndex].getKey() == "index")
// 			{
// 				std::vector<std::string> value = split(data[dataIndex].getValue(), " ");
// 				for (size_t k = 0; k < value.size(); k++)
// 				{
// 					fullPath = root + "/" + value[k];
// 					std::cout << fullPath << std::endl;
//                     if (access(fullPath.c_str(), R_OK | F_OK) == 0)
//                     {
//                         int fd;
//                         if ((fd = open(fullPath.c_str(), O_RDONLY)) != -1)
//                         {
//                             std::string str = readFile(fd);
//                             // this->body = str;
//                             return (1);
//                         }
//                     }
// 				}
// 			}
//             // std::cout << data[dataIndex].getKey() << "#" << data[dataIndex].getValue() << std::endl;
//         }
        
//     }
//     // this->statusCode = 404;
//     // this->version = req.getVersion();
//     // this->msg = "Not Found";
//     return (1);
// }

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


// void Response::checkPathname( const Request &req, const std::string& path )
// {
// 	if (req.getPathname() == path)
// 	{
		
// 	}
// }

// void Response::send( void ) {
// 	// (void)res;
// }

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

void Response::setRequest(const Request& req) {
	request = &req;
}
