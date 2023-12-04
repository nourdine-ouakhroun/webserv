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
		// std::cout << methode << std::endl;
		// std::cout << url << std::endl;
		// std::cout << version << std::endl;
		// std::cout <<  pathname<< std::endl;
		// std::cout << query << std::endl;
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
	std::string content;
	std::string key1;
	std::string key;
	std::string value;
	maps parseBody;
	std::vector<std::string> params;
	// std::cout  << "------------------------------------------" << std::endl;
	std::cout  << "#"<<  reqBody << "#" << std::endl;
	// std::cout  << "------------------------------------------" << std::endl;
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
				// if (!s.empty())
				// {
					if (!s[0].empty())
					{
						std::vector<std::string> s2 = split(s[0], "; ");
						for (size_t j = 0; j < s2.size(); j++)
						{
							if (j == 0) {
								std::vector<std::string> s3 = split(s2[0], ": ");
								key = s3[0];
								value = s3[1];
								// parseBody.insert(std::make_pair(key, value));
							}
							else {
								std::vector<std::string> s3 = split(s2[j], "=");
								// key = s3[0];
								key = s3[1].substr(1, s3[1].length() - 2);
								value = s[1];
							}
							std::cout << key << " " << value << std::endl;
							// parseBody.insert(std::make_pair(key, value));

							// std::cout << key << " " << << " " << value << std::endl;
						}
						
					}
					// if (!s[1].empty())
					// 	value = s[1];
					// parseBody.insert(std::make_pair(key1, value));
			}
				// }
				// }
				// for (size_t j = 0; j < s.size(); j++)
				// {
				// 	std::vector<std::string> s1 = split(s[j], "; ");
				// 	for (size_t l = 0; l < s1.size(); l++)
				// 	{
				// 		std::vector<std::string> s2;
				// 		if (l == 0)
				// 		{
				// 			s2 = split(s1[l], ": ");
				// 			if (!s2[0].empty())
				// 			{
				// 				key = s2[0];
				// 				if (!s2[1].empty())
				// 				{
				// 					value = s2[1];
				// 					parseBody.insert(std::make_pair(key, value));

				// 				}
				// 			}
				// 		}
				// 		// else
				// 		// {
				// 		// 	s2 = split(s1[l], "=");
				// 		// 	key = s2[1].substr(1, s2[1].length() - 2);
				// 		// 	std::cout << key << std::endl;
				// 		// }
				// 		// if (s2.size() == 1)
				// 		// {
				// 		// 	content = s2[0];
							// parseBody.insert(std::make_pair(key, content));
				// 		// 	// std::cout << key << " " << content << std::endl;
				// 		// }
				// 	}
				// 	// parseBody.insert(std::make_pair(key, content));

				// }
				for (maps::iterator it = parseBody.begin(); it != parseBody.end(); it++)
				{
					std::cout << it->first << " " << it->second << std::endl;
				}
				// key = ;
				// value = content;

				// std::cout << parseBody["Content-Disposition"] << std::endl;
				// std::cout << parseBody["name"] << std::endl;
				// std::cout << content << std::endl;
			// }
			// for (size_t i = 0; i < params.size(); i++)
			// {
			// 	// std::vector<std::string> s = split(sBoundary[i], "\n");

			// 	// if ()
			// 	std::cout << params[i] << std::endl;
			// }
			// reqBody.find(end) != std::string::npos;
			// while (reqBody.find(end) != std::string::npos);

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
// void Request::set(std::pair<std::string, std::string> keyvalue)
// {
// 	this->_header = keyValue
// }




// void     Request::parseRequest( std::string request )
// {
// 	size_t headerPos = request.find("\r\n\r\n");
// 	if (headerPos != std::string::npos)
// 	{
// 		size_t start = request.find("\r\n");
// 		if (start != std::string::npos)
// 			this->requestLine = request.substr(0, start);
// 		size_t i = start + 2;
// 		while (i < headerPos)
// 		{
// 			if (request[i] != '\r')
// 				this->requestHeader += request[i];
// 			i++;
// 		}
// 		i = headerPos + 4;
// 		while (i < request.length())
// 		{
// 			if (request[i] != '\r')
// 				this->requestBody += request[i];
// 			i++;
// 		}
// 	}
// 	// if (this->methode == "GET")
// 	// {
// 	// 	GetMethod	get;

// 	// 	get.parseRequestLine(this->requestLine);
// 	// 	get.parseHeader(this->requestHeader);
// 	// }
// 	// 	GetMethod get;
// 	// if (this->requestLine == "GET")
// 	// {
// 	// 	get.parseRequestLine(this->requestLine);
// 	// 	get.parseHeader(this->requestHeader);
// 	// }
// 	// 	return (get);
// 	// else if (this->methode == "POST")
// 	// {
// 	// 	PostMethod post;
// 	// 	post.parseRequestLine(this->requestLine);
// 	// }
// 	// else if (this->methode == "DELETE")
// 	// {
// 	// 	DeleteMethod Delete;
// 	// 	post.parseRequestLine(this->requestLine);
// 	// }

// 	// std::cout << "#" << this->requestLine << "#\n" << std::flush;
// 	// std::cout << "------------------------------\n" << std::flush;
// 	// std::cout << "$" << this->requestHeader << "$\n" << std::flush;
// 	// std::cout << "------------------------------\n" << std::flush;
// 	// std::cout << "&" << this->requestBody << "&\n" << std::flush;
// 	// std::cout << "------------------------------\n" << std::flush;


// 	// std::cout << "#" << requestLine[0] << " " << requestLine[1] << " " << requestLine[2] << "#" << std::endl;
// 	// std::cout << "&" << headerReq << "&" << std::endl;
// 	// std::cout << "$" << bodyReq << "$" << std::endl;
// }