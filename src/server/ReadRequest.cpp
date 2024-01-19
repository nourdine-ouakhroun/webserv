#include"ReadRequest.hpp"
#include<../Request/Request.hpp>
// ReadRequest::ReadRequest()
// {
// }
ReadRequest::ReadException::ReadException()
{
	this->exception_msg = "POLL EXCEPTION";
}
ReadRequest::ReadException::ReadException(const string &exception_msg)
{
	this->exception_msg = exception_msg;
}
const char *ReadRequest::ReadException::what() const throw()
{
	return this->exception_msg.c_str();
}
ReadRequest::ReadException::~ReadException() throw()
{
}
ReadRequest::ReadRequest(Socket &socket) : socket(socket)
{
}

ReadRequest::~ReadRequest()
{
}

void	ReadRequest::checkReqeust()
{
	
}

void	ReadRequest::Read()
{
	ssize_t		bytes;
	String		boundary;
	char		read_buffer[READ_NUMBER] = {0};

	bytes = 0;
	bytes = recv(socket.getFdPoll().fd, read_buffer, READ_NUMBER - 1, 0);
	if(bytes > 0)
	{
		string tmp_buffer (read_buffer, (size_t)bytes);
		if(socket.getHeader().empty() == true)
			setHeadre(tmp_buffer);
		socket.setBody(tmp_buffer);
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
					throw ReadRequest::ReadException();
			}
		}
		if((size_t)socket.getContenlenght() == socket.getBody().size())
			throw ReadRequest::ReadException();;
	}
	return ;
}

void	ReadRequest::setHeadre(string &buffer)
{
	Request tmp;
	size_t	pos = buffer.find("\r\n\r\n");
	if(pos == NPOS)
		throw runtime_error("");
	socket.setHeader(buffer.substr(0, pos + 4));
	tmp.parseRequest(socket.getHeader());
	if(tmp.getMethode() != "POST")
		throw ReadRequest::ReadException();
	if(tmp.header("Transfer-Encoding") == "chunked")
	{
		socket.hex_valeu = 2;
		socket.is_chuncked = true;
		buffer = buffer.substr(buffer.find("\r\n\r\n") + 2);
		return;
	}
	buffer = buffer.substr(buffer.find("\r\n\r\n") + 4);
	if(tmp.header("Content-Length").empty() == false)
		socket.setContenlenght(strtol(tmp.header("Content-Length").c_str(), NULL, 10));
	throw ReadRequest::ReadException();
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
