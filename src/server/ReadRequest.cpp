#include"ReadRequest.hpp"

// ReadRequest::ReadRequest()
// {
// }
ReadRequest::ReadRequest(Socket &socket) : socket(socket)
{
}

ReadRequest::~ReadRequest()
{
}

void	ReadRequest::checkReqeust()
{
	
}

void	ReadRequest::Request()
{
	ssize_t		bytes;
	String		boundary;
	char		buffer[READ_NUMBER] = {0};

	bytes = 0;
	// bzero(buffer, READ_NUMBER);
	bytes = recv(socket.getFdPoll().fd, buffer, READ_NUMBER - 1, 0);
	if(bytes > 0)
	{
		string request (buffer, (size_t)bytes);
		if(socket.getHeader().empty() == true)
			setHeadre(request);
		socket.setBody(request);
		if(socket.is_chuncked == true)
		{
			while (true)
			{
				if(socket.hex_valeu > socket.getBodyChange().size())
					break;
				size_t pos = socket.getBodyChange().find("\r\n", socket.hex_valeu);
				if(pos == NPOS)
					break;
				pos += 2;
				string hexa = socket.getBodyChange().substr(socket.hex_valeu, pos);
				socket.getBodyChange().erase(socket.hex_valeu - 2, pos - (socket.hex_valeu - 2));
				size_t decimal = static_cast<size_t>(strtol(hexa.c_str(), NULL, 16));
				socket.hex_valeu += decimal;
				if(decimal == 0)
					throw 200;
			}
		}
		if((size_t)socket.getContenlenght() == socket.getBody().size())
			throw 200;
	}
	return ;
}

void	ReadRequest::setHeadre(string &Request)
{
	size_t	pos = Request.find("\r\n\r\n");
	if(pos == NPOS)
		throw runtime_error("");
	socket.setHeader(Request.substr(0, pos + 4));
	pos = socket.getHeader().find("\r\n");
	socket.setMethod(headerMethod(socket.getHeader().substr(0, pos)));
	if(socket.getMethod() != POST)
		throw 200;
	pos = socket.getHeader().find("chunked");
	if(pos != NPOS)
	{
		socket.hex_valeu = 2;
		socket.is_chuncked = true;
		Request = Request.substr(Request.find("\r\n\r\n") + 2);
		return;
	}
	pos = Request.find("Content-Length: ") + 16;
	socket.setContenlenght(strtol(Request.substr(pos, Request.find("\r\n", pos) - pos).c_str(), NULL, 10));
	Request = Request.substr(Request.find("\r\n\r\n") + 4);
}
int	ReadRequest::headerMethod(String	line)
{
	if(line.empty() == true)
		throw runtime_error("");
	vector<String> splited = line.split();
	/**
	 * @todo protection
	*/
	if(splited[0] == "GET")
		return 0;
	if(splited[0] == "POST")
		return 1;
	if(splited[0] == "DELET")
		return 2;
	return 3;
}
