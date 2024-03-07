#include "Servers.hpp"
#include "../Request/Request.hpp"
#include "../Request/Response.hpp"



string makeRespose(const Socket &socket, const ServerData &serversData)
{
	(void)socket;
	(void)serversData;
	ServerPattern	server;
	

	Request			req;
	// cout << socket.getHeader() + socket.getBody() << endl;
	// cout << "-----------------------------------" << endl;
	req.parseRequest(socket.getHeader() + socket.getBody());
	server = ServerData::getServer(serversData, socket.ipAndPort, req.header("Host")).front();
	Response	res(req, server);
	res.setMimeType(server.mimeTypes);

	// cout << "Method: " << req.getMethod() << endl;
	// cout << "Method: " << req.getBody() << endl;

	string content;
	try
	{
		res.isFormed();
		res.isMatched();
        res.isMethodAllowed();
		res.isRedirected();
        res.whichMethod();
	}
	catch (int status)
	{
		// cout << "StatusCode: " << status << endl;
		string response = res.getResponse();

		if (response.empty()) {
			string file = res.pathToServe + res.isFound(res.pathToServe);
			res.setStatusCode(status);
			res.setMessage(res.getStatusMessage(status));
			res.setHeader("Content-Type", "text/html");

			string fileName = res.getErrorFile(status);
			if (!fileName.empty()) 
				res.redirection(301, fileName);
			else if (status != 200) {
				if (status >= 300 && status < 400)
					res.redirection(status, res.getRedirection());
				else
					res.setBody("<h1 style=\"text-align: center;\" >" + to_string(status) + " " + res.getStatusMessage(status) + "</h1>");
				res.setHeader("Content-Type", "text/html");
			}
			else {
			// 	// success OK
				if (res.getBody().empty()) {
					content = readF(file);
					res.setBody(content);
					res.setHeader("Content-Type", res.getMimeType(req.extention(file)));
					res.setHeader("Content-Length", to_string(content.size()));
				}
				else {
					res.setHeader("Content-Type", "text/html");
					res.setHeader("Content-Length", to_string(res.getBody().size()));
				}
			}
			res.makeResponse();
		}
	}
	return res.getResponse();
}


Servers::Servers(ServerData	srvers)
{
	this->servers = srvers;
}

Servers::~Servers(void)
{
}


size_t	Servers::SocketsSize(void) const 
{
	return this->master.size();
}

const Socket&	Servers::getSocket(size_t index) const 
{
	return this->master[index];
}

const vector<Socket>&	Servers::getMasterSockets(void) const 
{
	return this->master;
}

const string&	Servers::getHeader(size_t index) const
{
	return this->master[index].getHeader();
}



void	Servers::initSockets(vector<String>&	allport)
{
	for(size_t i = 0; i < allport.size();i++)
	{
		int	fd;
		try
		{
			fd = Server::setSocket();
			vector<String> infos = allport[i].split(':');
			Server::bindSocket(fd, infos.front(), (int)strtol(infos.back().c_str(), NULL, 10));
			Server::listenPort(fd);
		}
		catch(runtime_error &e)
		{
			cout << e.what() << endl;continue;
		}
		fdSockets.push_back(fd);
	}
}

// void	Servers::initSocketPort80(void)
// {
// 	int	fd;
// 	try
// 	{
// 		fd = Server::setSocket();
// 		Server::bindSocket(fd, 80);
// 		Server::listenPort(fd);
// 	}
// 	catch(runtime_error &e)
// 	{
// 		cout << e.what() << endl;return;
// 	}
// 	fdSockets.push_back(fd);
// }

vector<String> removeDuplicatePorts(const vector<String>& allPorts)
{
	vector<String> uniquePorts;
	vector<String>::const_iterator begin = allPorts.begin();
	vector<String>::const_iterator end = allPorts.end();
	while (begin != end)
	{
		if (find(begin + 1, end, *begin) == end)
			uniquePorts.push_back(*begin);
		begin++;
	}
	return (uniquePorts);
}

vector<String>	Servers::getAllPorts(void) const
{
	vector<String>			allport;
	vector <ServerPattern>	allservers = servers.getAllServers();
	for (size_t i = 0; i < allservers.size(); i++)
	{
		vector <Data> ports = allservers[i].getData("listen");
		for (size_t j = 0; j < ports.size(); j++)
			allport.push_back(ports[j].getValue().split().front());
	}
	// cout << "allport.size() : " << allport.size() << endl;
	allport = removeDuplicatePorts(allport);
	return allport;
}

void	Servers::runAllServers(void)
{
	/**
	 * @attention put allport in ManageServers header.
	*/
	allport = getAllPorts();
	initSockets(allport);
}


void	Servers::readyToWrite(size_t &index, vector<pollfd> &poll_fd)
{
	// size_t send_lenght = 2000;
	// if(send_lenght > master[index].respond.size())
	// 	send_lenght =  master[index].respond.size();
	ssize_t write_value = write(master[index].getFdPoll().fd, master[index].respond.c_str(), master[index].respond.size());
	if(write_value > 0)
	{
		master[index].respond.erase(0, (size_t)write_value);
		if(master[index].respond.empty() == true)
		{
			close(master[index].getFdPoll().fd);
			erase(index, master);
			erase(index, poll_fd);
			index--;
		}
	}
}

// void	Servers::setRespond(const string & respond, size_t index)
// {
// 	this->master[index].respond = respond;
// }

short	Servers::Revents(size_t index) const 
{
	return this->master[index].getFdPoll().revents;
}


void Servers::acceptConection(size_t index)
{
	int newfd = accept(fdSockets[index], NULL, NULL);
	if(newfd < 0)
		throw runtime_error("accept : filed!");
	/**
	 * @todo check fcntl fhm chno katakhd mziaaaaan
	*/
	fcntl(newfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	Socket tmp;
	tmp.setFdPoll(newfd, POLLIN);
	tmp.ipAndPort = allport[index];
	master.push_back(tmp);
}

/**
 *
 *
 *
 *
 *
 *chof daba hahia socket variable fih kolchi yak
 *dik sa3a bdl fdik function(makeRespose) kifma 3jbk
 *o wahd lblan socket had lvariabl 3lach 3tito like gha hit
 *nta ghatkhdm kolchi fwst had function yk o mnha ta CGI o CGI
 *ana dik sa3a ghanhtaj dak lbody so rah khas lbody ikon 3ndk ta fhad lfunction
 *bach fach ghatl9a nta bli rah post ghat3tih li (body) mtfahmin mamtfahminch soni liya hm hm hm
 *nta db biti tzid chi7aja fdak lcalss(Socket) zidha fih
 *
 *
 *
 * hanta hit 3arf dinmk at9oliya tani olach 3ati liya hadak kaml odak tkhrbi9 o z3t lwrdi
 * ila knti mabaghich had lobjet ta3i (socket nta tkhdm mno ga3
 * ghat9ad class whd akhor yak
 * oghat initi fih gha les variable lighatkhdm bihom fwst had function
 * b7a body size, header,ipAndPort, o lbody tahowa hit ghaykhso youslni l CGI ya3ni ghaykhso i7dr l3ndk
 * fhad lfunction mz1 wa ikon mz1 awla maykonch
 * ghandiroh bzz malk tkhl3na fasi dlhbs rah gha sakt lik a ghanod like ghanchtf 3lik mohim ma3lana
 *khdm o 9ad zmr o tl9na rah bghina nkhdmo, m3ak escanor 9ahir lfasa ylh ghiyrha
 *
 *
 * 
 * matnsach rani pushit
 *
 *
 *
 */


void Servers::readyToRead(size_t i, vector<pollfd> &poll_fd)
{
	for (size_t j = 0; j < fdSockets.size(); j++)
	{
		if(poll_fd[i].fd == fdSockets[j])
		{
			acceptConection(j);
			return;
		}
	}
	ReadRequest read_request(master[i]);
	try
	{
		read_request.Read();
	}
	catch (ReadRequest::ReadException)
	{
		master[i].respond = makeRespose(master[i], servers);
		master[i].setFdPoll(POLLOUT);
	}
}



void Servers::isSocketsAreReady(vector<pollfd> &poll_fd)
{
	for (size_t i = 0; i < master.size(); i++)
	{
		poll_fd.push_back(master[i].getFdPoll());
	}
	int pint = poll(&poll_fd[0], static_cast<nfds_t>(poll_fd.size()), 8000);
	if(pint == 0)
		throw Servers::PollException("Server reloaded");
	if(pint < 0)
		throw runtime_error("poll : poll was failed");
}

void	Servers::setMasterSockets(void)
{
	for (size_t i = 0; i < fdSockets.size(); i++)
	{
		Socket tmp;
		tmp.setFdPoll(fdSockets[i], POLLIN);
		master.push_back(tmp);
	}
}