#include"ReadRequest.hpp"
#include<../Request/Request.hpp>
// ReadRequest::ReadRequest()
// {
// }
ReadRequest::ReadException::ReadException()
{
	this->exception_msg = "Read Exception";
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


void ReadRequest::handelChunked()
{
	while (true)
	{
		if(socket.hex_valeu > socket.getBodyChange().size())
			break;
		size_t pos = socket.getBodyChange().find("\r\n", socket.hex_valeu);
		if(pos == NPOS)
			break;
		string hexa = socket.getBodyChange().substr(socket.hex_valeu, pos - socket.hex_valeu);
		pos += 2;
		/*Temporary Condition------------------*/
		if(!socket.hex_valeu)
			socket.hex_valeu = 2;
		socket.getBodyChange().erase(socket.hex_valeu - 2, pos - (socket.hex_valeu - 2));
		size_t decimal = static_cast<size_t>(strtol(hexa.c_str(), NULL, 16));
		socket.hex_valeu += decimal;
		if(decimal == 0)
			throw ReadRequest::ReadException();
	}
}
void ReadRequest::recvSomthing(char * buffer, size_t bytes)
{
	string request (buffer, bytes);
	if(socket.getHeader().empty() == true)
		setHeadre(request);
	socket.setBody(request);
	if(socket.is_chuncked == true)
		handelChunked();
	if((size_t)socket.getContenlenght() == socket.getBody().size())
		throw ReadRequest::ReadException();
}
void	ReadRequest::Read()
{
	ssize_t		bytes;
	String		boundary;
	char		read_buffer[READ_NUMBER] = {0};

	bytes = 0;
	bytes = recv(socket.getFdPoll().fd, read_buffer, READ_NUMBER - 1, 0);
	if(bytes > 0)
		recvSomthing(read_buffer, (size_t)bytes);
	return ;
}

size_t findSeparator(const string & buffer){
	size_t	pos = buffer.find("\r\n\r\n");
	if(pos == NPOS)
		throw runtime_error("");
	return pos;
}

void chackMethode(const string & buffer, Request &tmp_parser){
	tmp_parser.parseRequestLine(buffer.substr(0, buffer.find("\r\n")));
	if(tmp_parser.getMethod() != "POST")
		throw ReadRequest::ReadException();
}
void	ReadRequest::setHeadre(string &buffer)
{
	size_t	position = findSeparator(buffer);
	position += 4; //Skip Separator Characters
	Request tmp_parser;
	socket.setHeader(buffer.substr(0, position));
	chackMethode(buffer, tmp_parser);
	tmp_parser.parseRequest(socket.getHeader());
	if(tmp_parser.header("Transfer-Encoding") == "chunked")
		socket.is_chuncked = true;
	else if(tmp_parser.header("Content-Length").empty() == false)
		socket.setContenlenght(strtol(tmp_parser.header("Content-Length").c_str(), NULL, 10));
	else
		throw ReadRequest::ReadException();
	buffer = buffer.substr(position);
}


// ReadRequest::ReadException::ReadException()
// {
// 	this->exception_msg = "Read EXCEPTION";
// }
// ReadRequest::ReadException::ReadException(const string &exception_msg)
// {
// 	this->exception_msg = exception_msg;
// }
// const char *ReadRequest::ReadException::what() const throw()
// {
// 	return this->exception_msg.c_str();
// }
// ReadRequest::ReadException::~ReadException() throw()
// {
// }