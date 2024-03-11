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
		if(socket.hex_valeu > socket.changeRequest().size())
			break;
		size_t pos = socket.changeRequest().find("\r\n", socket.hex_valeu);
		if(pos == NPOS)
			break;
		string hexa = socket.changeRequest().substr(socket.hex_valeu, pos - socket.hex_valeu);
		socket.changeRequest().erase(socket.hex_valeu - 2, pos - (socket.hex_valeu - 2));
		size_t decimal = static_cast<size_t>(strtol(hexa.c_str(), NULL, 16));
		socket.hex_valeu += decimal;
		if(decimal == 0)
			throw ReadRequest::ReadException();
	}
}

void ReadRequest::recvSomthing(char * buffer, size_t bytes)
{
	string request (buffer, bytes);
	if (socket.getRequest().empty() == true)
		setHeadre(request);
	socket.setRequest(request);
	if(socket.is_chuncked == true)
		handelChunked();
	if ((size_t)socket.getContenlenght() == socket.getRequest().size()) {
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
	bytes = read(socket.getFdPoll().fd, buffer, READ_NUMBER - 1);
	if(bytes <= 0)
		throw closeException();
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
	position += 4;
	socket.setRequest(buffer.substr(0, position));
	methode(buffer, tmp_parser);
	socket.hex_valeu = position + 2; // for chuncked
	tmp_parser.parseRequest(socket.getRequest());
	if(tmp_parser.header("Transfer-Encoding") == "chunked")
		socket.is_chuncked = true;
	else if(tmp_parser.header("Content-Length").empty() == false)
		socket.setContenlenght((size_t)strtol(tmp_parser.header("Content-Length").c_str(), NULL, 10) + position);
	else
		throw ReadRequest::ReadException();
	buffer.erase(0, position);
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