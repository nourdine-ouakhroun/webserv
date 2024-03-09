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


void ReadRequest::handelChunked()
{
	while (true)
	{
		if(socket.hex_valeu > socket.changeBody().size())
			break;
		size_t pos = socket.changeBody().find("\r\n", socket.hex_valeu);
		if(pos == NPOS)
			break;
		string hexa = socket.changeBody().substr(socket.hex_valeu, pos - socket.hex_valeu);
		pos += 2;
		socket.changeBody().erase(socket.hex_valeu - 2, pos - (socket.hex_valeu - 2));
		size_t decimal = static_cast<size_t>(strtol(hexa.c_str(), NULL, 16));
		socket.hex_valeu += decimal;
		if(decimal == 0)
			throw ReadRequest::ReadException();
	}
}

void ReadRequest::recvSomthing(char * buffer, size_t bytes)
{
	string request (buffer, bytes);
	socket.setBody(request);
	if (socket.hex_valeu == false)
		setHeadre(request);
	if(socket.is_chuncked == true)
		handelChunked();
	if ((size_t)socket.getContenlenght() == socket.getBody().size()) {
		throw ReadRequest::ReadException();
	}
}


const string& ReadRequest::getRequest() const {
	return (request);
}

void	ReadRequest::Read()
{
	ssize_t		bytes;
	String		boundary;
	char		buffer[READ_NUMBER];
	// static int count = 0;

	bytes = 0;
	bytes = recv(socket.getFdPoll().fd, buffer, READ_NUMBER - 1, 0);
	if (bytes > 0)
		recvSomthing(buffer, (size_t)bytes);
}

size_t findSeparator(const string & buffer){
	size_t	pos = buffer.find("\r\n\r\n");
	if(pos == NPOS)
		throw runtime_error("");
	return pos;
}

void methode(const string & buffer, Request &tmp_parser){
	tmp_parser.parseRequestLine(buffer.substr(0, buffer.find("\r\n")));
	if(tmp_parser.getMethod() != "POST")
		throw ReadRequest::ReadException();
}
void	ReadRequest::setHeadre(string &buffer)
{
	size_t	position = findSeparator(buffer);
	Request	tmp_parser;
	string	tmpHeader(buffer.substr(0, position + 4));
	position += 4;
	socket.hex_valeu = position + 2; // for chuncked
	methode(buffer, tmp_parser);
	tmp_parser.parseRequest(tmpHeader);
	if(tmp_parser.header("Transfer-Encoding") == "chunked")
		socket.is_chuncked = true;
	else if(tmp_parser.header("Content-Length").empty() == false)
		socket.setContenlenght((size_t)strtol(tmp_parser.header("Content-Length").c_str(), NULL, 10) + position);
	else
		throw ReadRequest::ReadException();
}


ReadRequest::ReadException::ReadException()
{
	this->exception_msg = "Read EXCEPTION";
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