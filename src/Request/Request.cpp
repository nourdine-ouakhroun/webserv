#include "Request.hpp"

std::string	readRequest(int clientFd)
{
	std::string	request;
	int			byte = 0;
	int			readbyte = 1;
	char		buf[readbyte + 1];
	while(1)
	{
		byte = read(clientFd, buf, readbyte);
		if (byte <= 0)
			break ;
		buf[byte] = 0;
		request += buf;
		if (byte < readbyte)
			break ;
	}
	return (request);
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
		line = line.substr(pos + sep.length(), line.length());
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

void    Request::parseRequest(std::string request)
{
	std::vector<std::string>	reqLine;
	std::string 	            headerReq;
	std::string		            bodyReq;

	size_t headerPos = request.find("\r\n\r\n");
	if (headerPos != std::string::npos)
	{
		size_t i = 0;
		size_t start = request.find("\r\n");
		if (start != std::string::npos)
		{
			std::string line = request.substr(0, start);
				this->parseRequestLine(line);
		}
		while (i < headerPos)
		{
			if (request[i] != '\r')
				headerReq += request[i];
			i++;
		}
		i = headerPos + 4;
		while (i < request.length())
		{
			if (request[i] != '\r')
				bodyReq += request[i];
			i++;
		}
	}
	this->parseHeader(headerReq);
	this->parseBody(bodyReq);
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
void Request::parseBody( std::string reqBody )
{
	std::string key;
	std::string value;

	if (this->getMethod() == "POST" && this->header("Content-Type") == "application/x-www-form-urlencoded")
		this->query = reqBody;
	else if (this->getMethod() == "POST" && this->header("Content-Type") == "multipart/form-data")
	{
		std::string bondray = this->header("boundary");
		if (!bondray.empty())
		{
			std::string start = "--" + bondray;
			std::string end = start + "--";
			std::vector<std::string> sBoundary = split(reqBody, start);
			for (size_t i = 0; i < sBoundary.size() - 1; i++)
			{
				std::vector<std::string> s = split(sBoundary[i], "\n");
				if (!s.empty() && !s[0].empty())
				{
					size_t pos = 0;
					if ((pos = s[0].find("name")) != std::string::npos)
					{
						key = s[0].substr(pos + 6, (s[0].length() - (pos + 7)));
						if (!s[1].empty())
							value = s[1];
						_body.insert(std::make_pair(key, value));
						key.clear();
						value.clear();
					}
				}
			}
		}
	}
}

void Request::parseUrl()
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

const std::string &Request::header(const std::string &key)
{
	return (this->_header[key]);
}
const std::string &Request::body(const std::string &key)
{
	return (this->_body[key]);
}

const std::string &Request::getMethod() const
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

const maps &Request::getHeader( void ) const
{
	return (this->_header);
}
const maps &Request::getBody( void ) const
{
	return (this->_body);
}

