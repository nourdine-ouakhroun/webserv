#include "Request.hpp"

#define READ_SIZE 1000
Request::Request(void) {
}
Request::~Request(void) {
}
// ----------------------------------- parsing request -----------------------------------
void    Request::parseRequest( const std::string &request )
{
	size_t pos = request.find("\r\n\r\n");
	if (pos != string::npos) {
		Header = request.substr(0, pos);
		Body = request.substr(pos + 4, request.length() - pos);
	}

	size_t start = request.find("\r\n");
	if (start != std::string::npos)
		parseRequestLine(Header.substr(0, start));
	parseHeader();
	std::string encoding = header("Transfer-Encoding");
	if (!encoding.empty() && encoding == "chunked")
		parseBody();
}
void Request::parseRequestLine( const string& requestLine )
{
	if (!requestLine.empty())
	{
		std::vector<std::string> sp;
		sp = split(requestLine, " ");
		if (!sp.empty())
		{
			if (!sp[0].empty())
				this->method = sp[0];
			if (!sp[1].empty())
				parseUrl(sp[1]);
			if (!sp[2].empty())
				this->version = sp[2];
		}
	}
}
void Request::parseUrl( string url )
{
    std::string path;
    size_t pos = 0;
    if ((pos = url.find("?")) != std::string::npos)
	{
    	this->path = url.substr(0, pos);
        this->query = url.substr(pos + 1, url.length());
	}
	else
    	this->path = url.substr(0, url.length());
}
void Request::parseHeader()
{
    std::pair<std::string, std::string> keyValue;
	std::string							line;
	size_t								newlinePos = 0;
	size_t								pos = 0;

	this->_header.clear();
	while ((newlinePos = Header.find("\n")) != std::string::npos)
	{
		line = Header.substr(0, newlinePos);
		if ((pos = line.find(": ")) != std::string::npos)
		{
			keyValue.first = line.substr(0, pos);
			keyValue.second = line.substr(pos + 2, line.length() - pos);
			this->_header[keyValue.first] = keyValue.second;
		}
		Header = Header.substr(newlinePos + 1, Header.length());
	}
}
void Request::parseBody()
{
	size_t hex_valeu = 0;
	while (true)
	{
		size_t pos = Body.find("\r\n", hex_valeu);
		if(pos == std::string::npos)
			break;
		pos += 2;
		string hexa = Body.substr(hex_valeu, pos);
		Body.erase(hex_valeu - 2, pos - (hex_valeu - 2));
		size_t decimal = static_cast<size_t>(strtol(hexa.c_str(), NULL, 16));
		hex_valeu += decimal;
		if(decimal == 0)
			break ;
	}
}
// ---------------------------------------------------------------------------------------------

const std::string& Request::header(const std::string &key) const
{
	try {
		std::string value = _header.at(key);
		return (*new string(value));
	}
	catch(...){
		return (*new string(""));
	}
}

const std::string &Request::getMethod() const
{
    return (method);
}

const std::string &Request::getVersion() const
{
    return (version);
}
const std::string &Request::getPath() const
{
    return (path);
}

const std::string &Request::getQuery() const
{
    return (query);
}
const maps   &Request::getHeader( void ) const{
	return (_header);
}
const std::string   &Request::getBody( void ) const{
	return (Body);
}


std::string Request::extention( const string &path) const
{
	size_t pos = path.rfind(".");
	std::string extention;
	if (pos != std::string::npos)
		extention = path.substr(pos + 1, path.length() - pos);
	return (extention);
}
