#include "StatusCode.hpp"
#include "ManageServers.hpp"
std::string readFile(int fd)
{
    std::string		request;
	ssize_t			byte = 0;
	size_t			readbyte = 1024;
	char			buf[readbyte + 1];

	while(1)
	{
		byte = read(fd, buf, readbyte);
		if (byte <= 0)
            break;
		buf[byte] = 0;
		request += buf;
		if (byte < (ssize_t)readbyte)
			break ;
	}
	return (request);
}

StatusCode::StatusCode( void ) : statusCode(200), version("HTTP/1.1"), msg("OK")
{
}

StatusCode::~StatusCode( void )
{
}

void StatusCode::setStatusCode( const int &statusCode )
{
    this->statusCode = statusCode;
}
void StatusCode::setVersion( const std::string &version )
{
    this->version = version;
}
void StatusCode::setMsg( const std::string &msg )
{
    this->msg = msg;
}
const int &StatusCode::getStatusCode( void ) const
{
    return (this->statusCode);
}
const std::string &StatusCode::getVersion( void ) const
{
    return(this->version);
}
const std::string &StatusCode::getMsg( void ) const
{
    return (this->msg);
}
const std::string &StatusCode::getBody( void ) const
{
    return (this->body);
}





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


bool     StatusCode::isAllowed(const std::string &url)
{
	// std::cout << "|" << url << "|" << std::endl;
	for(size_t i = 0; i < url.length(); i++)
	{
		// std::cout << (!isprint(url[i])) << std::endl;
		if (!isprint(url[i]))
			return (0);
	}
	return (1);
}

bool StatusCode::isFormed(Request req)
{
    // std::cout << req.header("Transfer-Encoding") << std::endl;
    // std::cout << req.getMethod() << std::endl;
    if ((!req.header("Transfer-Encoding").empty() && req.header("Transfer-Encoding") != "chunked") || !this->isAllowed(req.getPathname()))
    {
        this->statusCode = 501;
        this->version = req.getVersion();
        this->msg = "Not Implemented";
        return (0);
    }
    else if (req.getMethod() == "GET" && req.header("Transfer-Encoding").empty() && req.header("Content-Length").empty())
    {
        this->statusCode = 400;
        this->version = req.getVersion();
        this->msg = "Bad Request";
        return (0);
    }
    else if (req.getPathname().length() > 2048)
    {
        this->statusCode = 414;
        this->version = req.getVersion();
        this->msg = "Request-URI To Long";
        return (0);
    }
    else if (!req.header("Content-Length").empty() && std::atoi(req.header("Content-Length").c_str()) > 1024 )
    {
        this->statusCode = 413;
        this->version = req.getVersion();
        this->msg = "Request Entity To Long";
        return (0);
    }
    return (1);
}

bool StatusCode::isMatched( const Request &req, std::vector<ServerModel> servers )
{
    // std::cout << "here" << std::endl;

    // std::cout << req.getHost() << "$"  << req.getPort() << std::endl;
    // std::cout << req.getHost() << "$"  << req.getPort() << std::endl;
    for (size_t server = 0; server < servers.size(); server++)
    {
        std::string fullPath;
		std::string root;
        std::vector<Data> data = servers[server].getLocationByPath(servers[server].getLocation(), req.getPathname()).getAllData();
        for (size_t dataIndex = 0; dataIndex < data.size(); dataIndex++)
        {
           if (data[dataIndex].getKey() == "root")
				root = data[dataIndex].getValue();
			else if (data[dataIndex].getKey() == "alias")
				root = data[dataIndex].getValue();
			else if (data[dataIndex].getKey() == "index")
			{
				std::vector<std::string> value = split(data[dataIndex].getValue(), " ");
				for (size_t k = 0; k < value.size(); k++)
				{
					fullPath = root + "/" + value[k];
					std::cout << fullPath << std::endl;
                    if (access(fullPath.c_str(), R_OK | F_OK) == 0)
                    {
                        int fd;
                        if ((fd = open(fullPath.c_str(), O_RDONLY)) != -1)
                        {
                            std::string str = readFile(fd);
                            this->body = str;
                            return (1);
                        }
                    }
				}
			}
            // std::cout << data[dataIndex].getKey() << "#" << data[dataIndex].getValue() << std::endl;
        }
        
    }
    // this->statusCode = 404;
    // this->version = req.getVersion();
    // this->msg = "Not Found";
    return (1);
}

