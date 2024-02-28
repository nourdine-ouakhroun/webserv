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
		Header = request.substr(0, pos + 2);
		Body = request.substr(pos + 4, request.length() - pos);
	}

	size_t start = request.find("\r\n");
	if (start != std::string::npos)
		parseRequestLine(Header.substr(0, start));
	
	if (!Header.empty())
		parseHeader();
	if (!Body.empty())
		parseBody();
}
void Request::unchunked() {
	size_t hex_value = 0;
	while (true)
	{
		size_t pos = Body.find("\r\n", hex_value);
		string hex = Body.substr(hex_value, pos - hex_value);
		Body.erase(hex_value, pos - hex_value + 2);
		size_t decimal = static_cast<size_t>(strtol(hex.c_str(), NULL, 16));
		hex_value += decimal + 2;
		if (decimal == 0)
			break ;
	}
}
void Request::parseRequestLine( const string& requestLine )
{
	if (!requestLine.empty())
	{
		std::vector<std::string> sp = split(requestLine, " ");
		// cout << sp.size() << endl;
		// for (size_t i = 0; i < sp.size(); i++)
		// {
		// 	cout << sp[i] << endl;
		// }
		
		if (!sp.empty())
		{
			if (!sp[0].empty()) {
				this->method = sp[0];
				if (!sp[1].empty()) {
					parseUrl(sp[1]);
					if (!sp[2].empty())
						this->version = sp[2];
				}
			}
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
	size_t								step = 0;

	this->_header.clear();
	while ((newlinePos = Header.find("\r\n", step)) != std::string::npos)
	{
		line = Header.substr(step, newlinePos - step);
		if ((pos = line.find(": ")) != std::string::npos)
		{
			keyValue.first = line.substr(0, pos);
			keyValue.second = line.substr(pos + 2, line.length() - pos + 2);
			this->_header[keyValue.first] = keyValue.second;
		}
		step = newlinePos + 2;
	}
	string content = header("Content-Type");
	if (!content.empty()) {
		vector<string> sp = split(content, "; ");
		if (sp.size() > 1) {
			contentType = sp[0];
			size_t pos = 0;
			if (!sp[1].empty() && (pos = sp[1].find("boundary")) != string::npos) {
				boundary = sp[1].substr(size_t(9), size_t(pos - 9));
			}
		}
	}
}




void Request::parseMultipartFormData(const std::string& body, const std::string& boundary) {
    std::vector<std::string>	parts;

    // Split body into parts based on boundary
    parts = split(body, boundary + "\r\n");
	for (size_t i = 0; i < parts.size(); i++)
	{
		string key;
		string content;
		parts[i] = parts[i].substr(0, parts[i].length() - 2);

		std::vector<std::string>	lines;
		if (!parts[i].empty())
    		lines = split(parts[i], "\r\n\r\n");

		for (size_t line = 0; line < lines.size(); line++) {
			if (line == 0) {
				std::vector<std::string> splitLine = split(lines[line], "; ");
				for (size_t j = 0; j < splitLine.size(); j++) {
					if (splitLine[j].find("=") != string::npos) {
						std::vector<std::string> keyValue = split(splitLine[j], "=");
						if (keyValue[0] == "filename") {
							std::vector<std::string> tmp = split(keyValue[1], "\r\n");
							key = tmp[0].substr(1, tmp[0].length() - 2);
						}
						else
							keyValue[1] = keyValue[1].substr(1, keyValue[1].length() - 2);
					}
				}
			}
			else {
				size_t lastBoundaryPos = lines[line].rfind(boundary);
				if (lastBoundaryPos != string::npos)
					lines[line].erase(lines[line].begin() + (int)lastBoundaryPos, lines[line].end());
				content = lines[line];
			}
		}
		upload.push_back(make_pair(key, content));
	}	
}
void Request::parseBody() {
	parseMultipartFormData(Body, "--" + getBoundary());
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
const std::string &Request::getBoundary() const
{
    return (boundary);
}
const std::string &Request::getContentType() const
{
    return (contentType);
}

const maps   &Request::getHeader( void ) const{
	return (_header);
}
const std::string   &Request::getBody( void ) const{
	return (Body);
}
const vector<pair<string, string> > Request::getUploads() const {
	return (upload);
}

std::string Request::extention( const string &path) const
{
	size_t pos = path.rfind(".");
	std::string extention;
	if (pos != std::string::npos)
		extention = path.substr(pos + 1, path.length() - pos);
	return (extention);
}
