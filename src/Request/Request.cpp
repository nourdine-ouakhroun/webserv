#include "Request.hpp"

std::vector<std::string> split(std::string line, std::string sep)
{
	std::vector<std::string> sp;
	if (line.empty())
		return (sp);
	size_t pos = 0;
	while ((pos = line.find(sep)) != std::string::npos)
	{
		std::string l = line.substr(0, pos);
		if (!l.empty())
			sp.push_back(l);
		pos += sep.length();
		line.erase(0, pos);
	}
	sp.push_back(line.substr(0, line.length()));
	return (sp);
}

Request::Request( void )
{
}
Request::~Request( void )
{
}

void    Request::parseRequest(const std::string &request)
{
	std::vector<std::string>	reqLine;
	std::string 	            headerReq;
	std::string		            bodyReq;

	size_t headerPos = request.find("\r\n\r\n");
	if (headerPos != std::string::npos)
	{
		size_t start = request.find("\r\n");
		if (start != std::string::npos)
		{
			std::string line = request.substr(0, start);
				this->parseRequestLine(line);
		}
		headerReq = request.substr(start + 2, headerPos);
		bodyReq = request[headerPos + 2];
		// bodyReq = request.substr(headerPos + 2, request.size());
	}
	this->parseHeader(headerReq);
	// this->parseBody(bodyReq);
	// this->parsePortAndHost();
}

void Request::parseRequestLine( std::string reqLine )
{
	if (!reqLine.empty())
	{
		std::vector<std::string> sp;
		sp = split(reqLine, " ");
		if (!sp.empty())
		{
			if (!sp[0].empty())
				this->method = sp[0];
			if (!sp[1].empty())
			{
				this->url = sp[1];
				this->parseUrl();
			}
			if (!sp[2].empty())
				this->version = sp[2];
		}
	}
}
void Request::parseHeader( std::string reqHeader )
{
    std::pair<std::string, std::string> keyValue;
	std::string							line;
	size_t								newlinePos = 0;
	size_t								pos = 0;

	this->_header.clear();
	while ((newlinePos = reqHeader.find("\n")) != std::string::npos)
	{
		line = reqHeader.substr(0, newlinePos);
		if ((pos = line.find(": ")) != std::string::npos)
		{
			keyValue.first = line.substr(0, pos);
			keyValue.second = line.substr(pos + 2, line.length());
			if (keyValue.first == "Content-Type")
			{
				std::vector<std::string> sp = split(keyValue.second, ";");
				if (!sp[0].empty())
					keyValue.second = sp[0].substr(0, sp[0].length());
				if (!sp[1].empty())
				{
					sp = split(sp[1].substr(1, sp[1].length()), "=");
					if (!sp[0].empty() && !sp[1].empty())
						this->_header[sp[0]] = sp[1].substr(0, sp[1].length());
				}
			}
			this->_header[keyValue.first] = keyValue.second; 
		}
		reqHeader = reqHeader.substr(newlinePos + 1, reqHeader.length());
	}
}
void Request::parseBody( std::string& reqBody )
{
	// std::string key;
	// std::string value;
	// this->_body.clear();
	// if (this->getMethode() == "POST" && this->header("Content-Type") == "application/x-www-form-urlencoded")
	// 	this->query = reqBody;
	// else if (this->getMethode() == "POST" && this->header("Content-Type") == "multipart/form-data")
	// {
	// 	std::string bondray = this->header("boundary");
	// 	if (!bondray.empty())
	// 	{
	// 		std::string start = "--" + bondray;
	// 		std::string end = start + "--";
	// 		std::vector<std::string> sBoundary = split(reqBody, start);
	// 		for (size_t i = 0; i < sBoundary.size() - 1; i++)
	// 		{
	// 			std::vector<std::string> s = split(sBoundary[i], "\n");
	// 			if (!s.empty() && !s[0].empty())
	// 			{
	// 				size_t pos = 0;
	// 				if ((pos = s[0].find("name")) != std::string::npos)
	// 				{
	// 					key = s[0].substr(pos + 6, (s[0].length() - (pos + 7)));
	// 					if (!s[1].empty())
	// 						value = s[1];
	// 					_body.insert(std::make_pair(key, value));
	// 					key.clear();
	// 					value.clear();
	// 				}
	// 			}
	// 		}
	// 	}
	// }
	size_t hex_valeu = 0;
	while (true)
	{
		size_t pos = reqBody.find("\r\n", hex_valeu);
		if(pos == std::string::npos)
			break;
		pos += 2;
		string hexa = reqBody.substr(hex_valeu, pos);
		reqBody.erase(hex_valeu - 2, pos - (hex_valeu - 2));
		size_t decimal = static_cast<size_t>(strtol(hexa.c_str(), NULL, 16));
		hex_valeu += decimal;
		if(decimal == 0)
			break ;
	}
}

void Request::parseUrl( void )
{
    std::string pathname;
    size_t pos = 0;
    if ((pos = this->url.find("?")) != std::string::npos)
	{
    	this->pathname = this->url.substr(0, pos);
        this->query = this->url.substr(pos + 1, this->url.length());
	}
	else
    	this->pathname = this->url.substr(0, this->url.length());
}

std::string Request::header(const std::string &key) const
{
	for (maps::const_iterator it = this->_header.begin(); it != this->_header.end(); it++)
	{
		if (it->first == key)
			return (it->second);
	}
	return ("");
}
std::string Request::body(const std::string &key) const
{
	for (maps::const_iterator it = this->_header.begin(); it != this->_header.end(); it++)
	{
		if (it->first == key)
			return (it->second);
	}
	return ("");
}



const std::string &Request::getMethode() const
{
    return (this->method);
}
const std::string &Request::getUrl() const
{
    return (this->url);
}
const std::string &Request::getVersion() const
{
    return (this->version);
}
const std::string &Request::getPathname() const
{
    return (this->pathname);
}
/*
void	Request::parsePortAndHost( void )
{
	std::vector<std::string> s = split(this->header("Host"), ":");
	if (!s.empty() && !s[0].empty() && !s[1].empty())
	{
		this->host = s[0];
		this->port = std::atoi(s[1].c_str());
	}
}
const std::string &Request::getHost( void ) const
{
	return (this->host);
}
const int         &Request::getPort( void ) const
{
	return (this->port);
}
*/
const std::string &Request::getQuery() const
{
    return (this->query);
}

const maps &Request::getHeader( void ) const
{
	return (this->_header);
}
std::string &Request::getBody( void )
{
	return (this->_body);
}

std::string Request::extention( const std::string &path) const
{
	size_t pos = path.rfind(".");
	std::string extention;
	if (pos != std::string::npos)
		extention = path.substr(pos + 1, path.length());
	return (extention);
}


// check request with config file
void Request::isFormed(ServerPattern srv, Response &res) {
	(void)res;
	(void)srv;
	if (!header("Transfer-Encoding").empty() && header("Transfer-Encoding") != "chunked")
		throw 500;
	else if (header("Transfer-Encoding").empty() && header("Content-Length").empty() && getMethode() == "Post")
		throw 400;
	else if (getPathname().length() > 2048)
		throw 414;
	else if (!srv.getData("client_max_body_size").empty() && std::atoi(header("Content-Lengt").c_str()) > (int)(std::atoi(srv.getData("client_max_body_size")[0].getValue().c_str()) / 100))
		throw 413;
}
void Request::isMatched(Response &res) {
	(void)res;
}
void Request::isRedirected(Response &res) {
	(void)res;
}
void Request::isAllowed(Response &res) {
	(void)res;
}
void Request::whichMethode(Response &res) {
	(void)res;
}


// Server Check
std::string Request::getRootFromLocation(ServerPattern server) {
	std::string			pathName = getPathname();
	std::vector<Data>	vRoot;
	static std::string	root;
	// bool				isFound = false;




	LocationPattern location = server.getLocationByPath(server.getLocations(), pathName);

	if (!location.getPath().empty()) {
		vRoot = location.getData("root");
		if (!vRoot.empty())
			root = vRoot[0].getValue() + pathName;
	}
	else {

		return(root + pathName);
	}
	return (root);
}
#include "String.hpp"

std::string Request::checkServer(const ServerPattern &server)
{
	bool		isFound = false;
	std::string	path = getPathname();
	std::string	root;
	std::string	fullPath;

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
	}
	vector<String>::iterator it = max_element(newPaths.begin(), newPaths.end());
	if (it == newPaths.end())
		throw 404;
	LocationPattern location = server.getLocationByPath(server.getLocations(), newPaths[0]);
	if (!location.getPath().empty()) {

		std::vector<Data> vRoot = location.getData("root");
		if (!vRoot.empty()) {
			root = vRoot[0].getValue();
			if (isDirectory(root + getPathname()) && (root + getPathname()).back() != '/' )
				throw 301;
		}
		if (!isDirectory(root + path)) {
			return (root + path);
		}
		std::vector<Data> vIndex = location.getData("index");
		if (!vIndex.empty())
		{
			for (size_t i = 0; (i < vIndex.size() && !isFound); i++)
			{
				std::vector<std::string> filename = split(vIndex[i].getValue(), " ");
				for (size_t j = 0; (j < filename.size() && !isFound); j++)
				{
					std::string fileExist = root + path + filename[j];
					if (access(fileExist.c_str(), O_RDONLY) != -1)
					{
						fullPath = fileExist;
						isFound = true;
					}
				}
			}
		}
	}
	return (fullPath);
}

int Request::isDirectory(const std::string& path)
{
	struct stat statbuf;
	if (stat(path.c_str(), &statbuf) != 0)
		return 0;
	return S_ISDIR(statbuf.st_mode);
}
