#include "Request.hpp"

#define READ_SIZE 1000
std::string Request::readFile(const std::string &path)
{
	std::string content;
	char buffer[READ_SIZE + 1];

	int fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
		return (content);

	while (1)
	{
		ssize_t readBytes = read(fd, buffer, READ_SIZE);
		if (readBytes <= 0)
			break;
		buffer[readBytes] = 0;
		content.append(buffer, (u_long)readBytes);
	}
	close(fd);
	return (content);
}

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

Request::Request(void) {

}

Request::~Request(void) {
}

// "Page Is Forbidden"
// "<h1>Page Is Forbidden</h1>"
// "Bad Request"
// "<h1>Bad Request</h1>"
// "Request-Uri Too Longe"
// "<h1>Request-Uri Too Longe<h1>"
// "Request Entity Too Longe"
// "<h1>Request Entity Too Longe<h1>"
// "Move Permanently"
// "<h1>Not Implamented</h1>"

// setHeader("Location", request->getPathname() + "/");




void    Request::parseRequest( const std::string &req)
{
	std::vector<std::string>	reqLine;
	std::string body;


	size_t headerPos = req.find("\r\n\r\n");
	if (headerPos != std::string::npos)
	{
		size_t start = req.find("\r\n");
		if (start != std::string::npos)
		{
			std::string line = req.substr(0, start);
			this->parseRequestLine(line);
		}
		_request = req.substr(start + 2, headerPos - start);
		_body = req.substr(headerPos + 2, req.size() - headerPos);
		parseHeader();
		std::string encoding  = header("Transfer-Encoding");
		if (!encoding.empty() && encoding == "chunked")
			parseBody();
	}
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
void Request::parseHeader( void )
{
    std::pair<std::string, std::string> keyValue;
	std::string							line;
	size_t								newlinePos = 0;
	size_t								pos = 0;
	std::string							request = _request;

						  this->_header.clear();
	while ((newlinePos = request.find("\n")) != std::string::npos)
	{
		line = request.substr(0, newlinePos);
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
		request = request.substr(newlinePos + 1, request.length());
	}
}
void Request::parseBody( void )
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
		size_t pos = _body.find("\r\n", hex_valeu);
		if(pos == std::string::npos)
			break;
		pos += 2;
		string hexa = _body.substr(hex_valeu, pos);
		_body.erase(hex_valeu - 2, pos - (hex_valeu - 2));
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

std::string Request::header(const std::string &key)
{
	try {
		std::string value = _header.at(key);
		return (value);
	}
	catch(...){
		return ("");
	}
}
// std::string Request::body(const std::string &key) const
// {
// 	for (maps::const_iterator it = this->_header.begin(); it != this->_header.end(); it++)
// 	{
// 		if (it->first == key)
// 			return (it->second);
// 	}
// 	return ("");
// }

void Request::setServer(const ServerPattern &server)
{
	this->server = server;
}



void Request::setLocation(const LocationPattern &location)
{
	this->location = location;
}

const ServerPattern &Request::getServer(void) const
{
	return (server);
}

const LocationPattern &Request::getLocation(void) const
{
	return (location);
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

const std::string &Request::getQuery() const
{
    return (this->query);
}

const std::string &Request::getRequest(void) const
{
	return (this->_request);
}
const std::string &Request::getBody( void ) const
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
void Request::isFormed( ) {
	// cout << getMethode() << endl;

	if (!header("Transfer-Encoding").empty() && header("Transfer-Encoding") != "chunked") throw 500;
	else if ((header("Transfer-Encoding").empty() && header("Content-Length").empty() && getMethode() == "POST"))
		throw 400;
	else if (getPathname().length() > 2048)
		throw 414;
	else if (!server.getData("client_max_body_size").empty()
	&& std::atoi(header("Content-Lengt").c_str()) > (int)(std::atoi(server.getData("client_max_body_size")[0].getValue().c_str()) / 100))
		throw 413;
}
void Request::isMatched(Response &res ) {

	std::string pathToServe = getFullPath();
	if (access(pathToServe.c_str(), O_RDONLY) == -1)
		throw 404;
	string content = Request::readFile(pathToServe);
	res.setBody(content);
	res.setHeader("Content-Length", to_string(content.size()));
	res.setHeader("Content-Type", res.getMimeType(extention(getPathname())));
	res.setResponse(200);

}
void Request::isRedirected(Response &res) {
	(void)res;
}
void Request::isMethodAllowed() {
	// server.getAllLocationPath();
	// LocationPattern loca = getLocation();
	vector<Data> method = location.getData("method");
	if (!method.empty()) {
		vector<string> methods = split(method.front().getValue(), " ");
		if (find(methods.begin(), methods.end(), getMethode()) == methods.end())
			throw 405;
	}
}
void Request::whichMethode(Response &res) {
	(void)res;
}

std::string Request::getFullPath()
{
	bool isFound = false;
	std::string path = getPathname();
	std::string root;
	std::string fullPath;

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
	location = server.getLocationByPath(server.getLocations(), newPaths[0]);
	// setLocation(server.getLocationByPath(server.getLocations(), newPaths[0]));
	if (!location.getPath().empty())
	{
		std::vector<Data> vRoot = location.getData("root");
		if (!vRoot.empty())
		{
			root = vRoot[0].getValue();
			if (isDirectory(root + getPathname()) == 1 && (root + getPathname()).back() != '/')
				throw 301;
		}
		if (!(isDirectory(root + path) == 1))
		{
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
std::string Request::getErrorFile(int statusCode) const {
	vector<Data>  errorPage = server.getData("error_page");

	for (size_t i = 0; i < errorPage.size(); i++) {
		vector<string> value = split(errorPage[i].getValue(), " ");
		for (size_t j = 0; j < value.size() - 1; j++) {
			if (statusCode == atoi(value[j].c_str()))
				return (value[value.size() - 1]);
		}
	}
	return ("");
}

// std::string Request::checkServer( void ) {

// 	std::string fullPath = getFullPath();
// 	cout << "path = " << fullPath << endl;
// 	if (access(fullPath.c_str(), O_RDONLY) == -1)
// 		throw 404;
// 	return (fullPath);
// }

int Request::isDirectory(const std::string& path) const
{
	struct stat statbuf;
	if (stat(path.c_str(), &statbuf))
		return (-1);
	return S_ISDIR(statbuf.st_mode);
}





