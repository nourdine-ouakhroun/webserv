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

void ReadRequest::checkReqeust()
{
	
}

			// int l = 0;
void	ReadRequest::Request()
{
	ssize_t		bytes;
	String		boundary;
	char		buffer[READ_NUMBER];

	bytes = 0;
	memset(buffer, 0, READ_NUMBER);
	bytes = recv(socket.getFdPoll().fd, buffer, READ_NUMBER - 1, 0);
	// cout << "bytes : " << bytes << endl;
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
					throw 1;
			}
		}
		if((size_t)socket.getContenlenght() == socket.getBody().size())
			throw 1;
	}
	// cout << socket.getLenght() << " : " << socket.getContenlenght() << endl;
	// if(socket.getLenght() != socket.getContenlenght() || !socket.rest.empty())
	// 	throw runtime_error("");
	return ;
}


// static int l;
// void	read_request::handelChunked(string &Request)
// {
// 	string tmpRequest;
// 	// cout << "////////////befor : " << Request << "////////////" << endl;
// 	// ofstream fileB("/Users/nouakhro/Desktop/subjects/webserv/befor", fstream::app);
// 	// fileB << Request << endl;
// 	// exit(1);
// 	while (!Request.empty())
// 	{
// 		if(socket.hex_valeu == 0)
// 		{
// 			size_t pos = Request.find("\r\n");
// 			if(pos == NPOS)
// 			{
// 				tmpRequest.append(Request);
// 				Request.erase(0, Request.size());
// 				socket.hex_valeu = 0;
// 				break;
// 			}
// 			string hex = Request.substr(0, pos);
// 			socket.hex_valeu = (size_t)HexToDecimal(hex);
// 			Request.erase(0, pos + 2);
// 		}
// 		if(socket.hex_valeu > Request.size())
// 		{
// 			tmpRequest.append(Request);
// 			socket.hex_valeu -= Request.size();
// 			Request.erase(0, Request.size());
// 			break;
// 		}
// 		tmpRequest.append(Request.substr(0, socket.hex_valeu));
// 		Request.erase(0, socket.hex_valeu + 2);
// 		socket.hex_valeu = 0;
// 	}
// 	Request = tmpRequest;
// 	// ofstream fileA("/Users/nouakhro/Desktop/subjects/webserv/after", fstream::app);
// 	// fileA << Request << endl;
// 	// if(l >= 35)
// 	// 	exit(1);
// 	l++;
// }

// void	read_request::removePartOfupload()
// {
// 	size_t	begin;

// 	if(socket.getBoundary().empty() == false)
// 	{
// 		if(socket.status == PUTINSTRING)
// 			putInString();
// 		else if(socket.status == PUTINFILE)
// 			putInFile();
// 		else if(socket.status == DEFAULT && (begin = socket.rest.find(socket.getBoundary())) != NPOS)
// 			checkIfFile(begin);
// 	}
// 	else
// 	{
// 		socket.setHeader(socket.rest);
// 		socket.appendLenght(socket.rest.size());
// 	}
// }
// static int i;
// void	read_request::checkIfFile(size_t	&begin)
// {
// 	string tmp_string;
// 	try
// 	{
//     	removeBoundary(tmp_string, begin);
// 	}
// 	catch(const string& e)
// 	{
// 		cerr << e << '\n';
// 		return;
// 	}
	
// 	size_t	pos = tmp_string.find("filename=\"", begin);
// 	if(pos != NPOS)
// 	{
// 		string filename(tmp_string.substr(pos + 10, tmp_string.find("\"", pos + 10) - (pos + 10)));
// 		socket.setFileName(filename);
// 		int	fd = open(filename.c_str(), O_CREAT | O_RDWR | O_APPEND, 0777);
// 		socket.setFd(fd);
// 		socket.status = PUTINFILE;
// 		return ;
// 	}
// 	socket.setHeader(tmp_string);
// 	socket.status = PUTINSTRING;
// 	return;
// }

// void    read_request::removeBoundary(string & tmp_string, size_t	&begin)
// {
// 	cout << socket.rest << endl;
// 	if(socket.rest.find(socket.getBoundary() + "--") == begin)
// 	{
// 		socket.appendLenght(socket.rest.size());
// 		socket.setHeader(socket.rest);
// 		socket.rest.erase(0, socket.rest.size());
// 		exit(1);
// 		throw "end of string";
// 	}
// 	begin = socket.rest.rfind("\r\n", begin);
// 	if(begin == NPOS)
// 		begin = 0;
// 	size_t	end = socket.rest.find("\r\n\r\n", begin + 2);
// 	if(end == NPOS)
// 		throw runtime_error("");
// 	else
// 		end += 4;
// 	tmp_string = socket.rest.substr(begin,  end  - begin);
// 	socket.rest.erase(begin, end  - begin);
// 	socket.appendLenght((end  - begin));
// }
// void	read_request::putInFile()
// {
// 	size_t	begin;
// 	if((begin = socket.rest.find(socket.getBoundary())) != NPOS)
// 	{
// 		size_t	end = socket.rest.rfind("\r\n", begin);
// 		if(end == NPOS)
// 			end = 0;
// 		socket.appendLenght(end);
// 		write(socket.getFd(), socket.rest.substr(0, end).c_str(), socket.rest.substr(0, end).size());
// 		close(socket.getFd());
// 		socket.rest.erase(0, end);
// 		socket.status = DEFAULT;
// 		return;
// 	}
// 	size_t	end = socket.rest.rfind("\r\n");
// 	if(end == NPOS || end == 0)
// 		throw runtime_error("");
// 	write(socket.getFd(), socket.rest.substr(0, end).c_str(), socket.rest.substr(0, end).size());
// 	socket.appendLenght(end);
// 	socket.rest.erase(0, end);
// 	return;
// }

// void	read_request::putInString()
// {
// 	size_t	begin;

// 	if((begin = socket.rest.find(socket.getBoundary())) != NPOS)
// 	{
// 		size_t	end = socket.rest.rfind("\r\n", begin);
// 		if(end == NPOS)
// 			end = 0;
// 		socket.setHeader(socket.rest.substr(0, end));
// 		socket.appendLenght(end);
// 		socket.rest.erase(0, end);
// 		socket.status = DEFAULT;
// 		return ;
// 	}
// 	size_t	end = socket.rest.rfind("\r\n");
// 	if(end == NPOS || end == 0)
// 		throw runtime_error("");
// 	socket.setHeader(socket.rest.substr(0, end));
// 	socket.appendLenght(end);
// 	socket.rest.erase(0, end);
// }
void	ReadRequest::setHeadre(string &Request)
{
	size_t	pos = Request.find("\r\n\r\n");
	if(pos == NPOS)
		throw runtime_error("");
	socket.setHeader(Request.substr(0, pos + 4));
	pos = socket.getHeader().find("\r\n");
	socket.setMethod(headerMethod(socket.getHeader().substr(0, pos)));
	if(socket.getMethod() != POST)
	{
		socket.respond = "HTTP/1.1 200 OK\r\n\r\n <h1> hello </h1>";
		throw 1;
	}
	pos = Request.find("boundary=") + 9;
	socket.setBoundary("--" + Request.substr(pos, Request.find("\r\n", pos) - pos));
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
void ReadRequest::postUtils()
{
	size_t	pos = socket.getHeader().find("chunked");
	if(pos != NPOS)
		socket.is_chuncked = true;

	// pos = socket.getHeader().find("Content-Length: ");
	// if(pos == NPOS)
	// 	return ;
	// long lenght = strtol(socket.getHeader().substr(pos + 16).c_str(), NULL, 10);
	// socket.setContenlenght(static_cast<ssize_t>(lenght));
	// pos = socket.getHeader().find("boundary=");
	// if(pos != NPOS)
	// {
	// 	string boundary = socket.getHeader().substr(pos + 9, socket.getHeader().find("\r\n", pos) - (pos + 9));
	// 	boundary.insert(0, "--");
	// 	socket.setBoundary(boundary);
	// }
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

// void	ReadRequest::methodSerch(string &Request)
// {
// 	size_t pos = Request.find("\r\n");
// 	if(pos != NPOS)
// 	{
// 		return;
// 	}
// 	throw runtime_error("");
// }