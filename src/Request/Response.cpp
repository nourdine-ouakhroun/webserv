#include "Response.hpp"
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

Response::Response( void ) : statusCode(200), msg("OK")
{
}

Response::~Response( void )
{
}

// seters
void Response::setStatusCode( const int &statusCode )
{
    this->statusCode = statusCode;
}
// void Response::setVersion( const std::string &version )
// {
//     this->version = version;
// }
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

void	Response::makeHeaderResponse( void )
{
	response += VERSION + std::to_string(getStatusCode()) + " " + getMsg() + ENDLINE;
	if (!_header.empty()) {
		for( maps::iterator it = _header.begin(); it != _header.end(); it++) {
			response += it->first + ": " + it->second + ENDLINE; 
		}
	}
}
void	Response::makeBodyResponse( void )
{
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

// bool Response::isMatched( const Request &req, std::vector<ServerModel> servers )
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

