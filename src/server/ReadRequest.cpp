#include"ReadRequest.hpp"

// ReadRequest::ReadRequest()
// {
// }
ReadRequest::ReadRequest(SocketDependencies &socket) : socket(socket)
{
}

ReadRequest::~ReadRequest()
{
}

void ReadRequest::checkReqeust()
{
	
}
// static int j;

void	ReadRequest::Requist()
{
	ssize_t		bytes;
	String		boundary;
	char		buffer[READ_NUMBER] = {0};

	bytes = 0;
	// bzero(buffer, READ_NUMBER);
	bytes = recv(socket.getFdPoll().fd, buffer, READ_NUMBER - 1, 0);
	if(bytes == 0)
	{
		if(socket.status == PUTINFILE)
			remove(socket.getFileName().c_str());
		socket.setContenlenght(socket.getLenght());
		socket.rest.clear();
	}
	else if(bytes > 0)
	{
		// if(bytes < 0)
		// 	bytes = 0;
		string request (buffer, (size_t)bytes);
		if(socket.getMethod() == -1)
			methodSerch(request);
		if(socket.getRequist().empty() == true)
			setOnlyHeadre(request);
		if(socket.ischunked == true)
			handelChunked(request);
		request.insert(0, socket.rest);
		socket.rest = request;
		// if(j >= 1)
		// {
		// 	exit(1);
		// }
		// j++;
		while (!socket.rest.empty())
		{
			if(socket.getMethod() == POST)
				removePartOfupload();
		}
	}
	if(socket.getLenght() != socket.getContenlenght() || !socket.rest.empty())
		throw runtime_error("");
	return ;
}

int	HexToDecimal(string hex)
{
   	int decimal;
    stringstream stream(hex);
    stream >> hex >> decimal;
    return decimal;
}
static int l;
void	ReadRequest::handelChunked(string &Requist)
{
	string tmpRequest;
	// cout << "////////////befor : " << Requist << "////////////" << endl;
	// ofstream fileB("/Users/nouakhro/Desktop/subjects/webserv/befor", fstream::app);
	// fileB << Requist << endl;
	// exit(1);
	while (!Requist.empty())
	{
		if(socket.hex_valeu == 0)
		{
			size_t pos = Requist.find("\r\n");
			if(pos == NPOS)
			{
				tmpRequest.append(Requist);
				Requist.erase(0, Requist.size());
				socket.hex_valeu = 0;
				break;
			}
			string hex = Requist.substr(0, pos);
			socket.hex_valeu = (size_t)HexToDecimal(hex);
			Requist.erase(0, pos + 2);
		}
		if(socket.hex_valeu > Requist.size())
		{
			tmpRequest.append(Requist);
			socket.hex_valeu -= Requist.size();
			Requist.erase(0, Requist.size());
			break;
		}
		tmpRequest.append(Requist.substr(0, socket.hex_valeu));
		Requist.erase(0, socket.hex_valeu + 2);
		socket.hex_valeu = 0;
	}
	Requist = tmpRequest;
	// ofstream fileA("/Users/nouakhro/Desktop/subjects/webserv/after", fstream::app);
	// fileA << Requist << endl;
	// if(l >= 35)
	// 	exit(1);
	l++;
}

void	ReadRequest::removePartOfupload()
{
	size_t	begin;

	if(socket.getBoundary().empty() == false)
	{
		if(socket.status == PUTINSTRING)
			putInString();
		else if(socket.status == PUTINFILE)
			putInFile();
		else if(socket.status == DEFAULT && (begin = socket.rest.find(socket.getBoundary())) != NPOS)
			checkIfFile(begin);
	}
	else
	{
		socket.setRequist(socket.rest);
		socket.appendLenght(socket.rest.size());
	}
}
// static int i;
void	ReadRequest::checkIfFile(size_t	&begin)
{
	string tmp_string;
	try
	{
    	removeBoundary(tmp_string, begin);
	}
	catch(const string& e)
	{
		cerr << e << '\n';
		return;
	}
	
	size_t	pos = tmp_string.find("filename=\"", begin);
	if(pos != NPOS)
	{
		string filename(tmp_string.substr(pos + 10, tmp_string.find("\"", pos + 10) - (pos + 10)));
		socket.setFileName(filename);
		int	fd = open(filename.c_str(), O_CREAT | O_RDWR | O_APPEND, 0777);
		socket.setFd(fd);
		socket.status = PUTINFILE;
		return ;
	}
	socket.setRequist(tmp_string);
	socket.status = PUTINSTRING;
	return;
}

void    ReadRequest::removeBoundary(string & tmp_string, size_t	&begin)
{
	cout << socket.rest << endl;
	if(socket.rest.find(socket.getBoundary() + "--") == begin)
	{
		socket.appendLenght(socket.rest.size());
		socket.setRequist(socket.rest);
		socket.rest.erase(0, socket.rest.size());
		exit(1);
		throw "end of string";
	}
	begin = socket.rest.rfind("\r\n", begin);
	if(begin == NPOS)
		begin = 0;
	size_t	end = socket.rest.find("\r\n\r\n", begin + 2);
	if(end == NPOS)
		throw runtime_error("");
	else
		end += 4;
	tmp_string = socket.rest.substr(begin,  end  - begin);
	socket.rest.erase(begin, end  - begin);
	socket.appendLenght((end  - begin));
}
void	ReadRequest::putInFile()
{
	size_t	begin;
	if((begin = socket.rest.find(socket.getBoundary())) != NPOS)
	{
		size_t	end = socket.rest.rfind("\r\n", begin);
		if(end == NPOS)
			end = 0;
		socket.appendLenght(end);
		write(socket.getFd(), socket.rest.substr(0, end).c_str(), socket.rest.substr(0, end).size());
		close(socket.getFd());
		socket.rest.erase(0, end);
		socket.status = DEFAULT;
		return;
	}
	size_t	end = socket.rest.rfind("\r\n");
	if(end == NPOS || end == 0)
		throw runtime_error("");
	write(socket.getFd(), socket.rest.substr(0, end).c_str(), socket.rest.substr(0, end).size());
	socket.appendLenght(end);
	socket.rest.erase(0, end);
	return;
}

void	ReadRequest::putInString()
{
	size_t	begin;

	if((begin = socket.rest.find(socket.getBoundary())) != NPOS)
	{
		size_t	end = socket.rest.rfind("\r\n", begin);
		if(end == NPOS)
			end = 0;
		socket.setRequist(socket.rest.substr(0, end));
		socket.appendLenght(end);
		socket.rest.erase(0, end);
		socket.status = DEFAULT;
		return ;
	}
	size_t	end = socket.rest.rfind("\r\n");
	if(end == NPOS || end == 0)
		throw runtime_error("");
	socket.setRequist(socket.rest.substr(0, end));
	socket.appendLenght(end);
	socket.rest.erase(0, end);
}

void	ReadRequest::setOnlyHeadre(string &Requist)
{
	size_t	pos = Requist.find("\r\n\r\n");
	if(pos == NPOS)
		throw runtime_error("");
	socket.setRequist(Requist.substr(0, pos + 4));
	Requist = Requist.substr(pos + 4);
	if(socket.getMethod() == POST)
		postUtils();
	socket.setLenght(0);
}
void ReadRequest::postUtils()
{
	size_t	pos = socket.getRequist().find("chunked");
	if(pos != NPOS)
		socket.ischunked = true;
	pos = socket.getRequist().find("Content-Length: ");
	if(pos == NPOS)
		return ;
	long lenght = strtol(socket.getRequist().substr(pos + 16).c_str(), NULL, 10);
	socket.setContenlenght(static_cast<ssize_t>(lenght));
	pos = socket.getRequist().find("boundary=");
	if(pos != NPOS)
	{
		string boundary = socket.getRequist().substr(pos + 9, socket.getRequist().find("\r\n", pos) - (pos + 9));
		boundary.insert(0, "--");
		socket.setBoundary(boundary);
	}
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

void	ReadRequest::methodSerch(string &Requist)
{
	size_t pos = Requist.find("\r\n");
	if(pos != NPOS)
	{
		socket.setMethod(headerMethod(Requist.substr(0, pos)));
		return;
	}
	throw runtime_error("");
}