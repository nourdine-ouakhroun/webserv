#include"Server.hpp"
#include <sys/stat.h>

ServerRun::ServerRun(ServerData	smodel)
{
	this->servers = smodel;
}
int	ServerRun::Newsocket()
{
	int socketfd = socket(PF_INET, SOCK_STREAM, 0);
	if(socketfd < 0)
	{
		std::cout << "error : socket conection not open ." << std::endl;
		exit(1);
	}
	int option = 1;
	setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));
	return socketfd;
}
void printreq(_requset _requisteconten)
{
	std::cout << "requist line :" << std::endl;
	for (size_t i = 0;i < _requisteconten.requistLine.size();i++)
	{
		std::cout <<"[ " << _requisteconten.requistLine[i] << " ]" << std::endl;
	}
	std::cout << "headre :" << std::endl;
	for (std::map<String, std::vector<String> >::iterator it = _requisteconten.header.begin(); it != _requisteconten.header.end(); it++)
	{
		std::cout << "Key : [ " <<  it->first << " ] ";
		std::cout << "Valeus : [";
		for (size_t i = 0; i < it->second.size(); i++)
		{
			std::cout << it->second[i] << " ";
		}
		std::cout << " ]" << std::endl;
	}
	std::cout << "body :" << std::endl;
	for (size_t i = 0;i < _requisteconten.body.size();i++)
	{
		std::cout <<"[ " << _requisteconten.body[i] << " ]" << std::endl;
	}
}

String getrespond(const Location & _location, const String &path)
{
	String						respond(ERROR_404);
	std::vector<Data> _data = _location.getAllData();
	String Allpath;
	std::vector<Data> roots = _location.getData("root");
	/**
	 * @attention exit if non root
	*/
	if (roots.empty() == true)
		return respond;
	Allpath = String(roots.at(0).getValue()).append(_location.getPath()).append("/");
	std::vector<Data> indexs;
	if(path.empty())
		indexs = _location.getData("index");
	else
		indexs.push_back(Data("index", path));
	if(indexs.empty() == true)
		return respond;
	for (size_t i = 0; i < indexs.size(); i++)
	{
		std::vector<String> index = String(indexs[i].getValue()).split();
		for (size_t j = 0; j < index.size(); j++)
		{
			String tmp(Allpath);
			tmp.append(index[j]);
			int fd = open(tmp.c_str() , O_RDONLY);
			if(fd < 0)
				continue;
			if(tmp.find('.') != SIZE_T_MAX && tmp.substr(tmp.find('.') + 1) != "html")
			{
				Cgi CgiScript(tmp);
				std::string responCgi = CgiScript.HandelScript();
				respond = responCgi;
				return respond;
			}
			respond.clear();
			char res[200];
			bzero(res, 200);
			ssize_t bytes = 0;
			while((bytes = read(fd, res, 199)) != 0)
			{
				respond.append(res);
				bzero(res, 200);
				if (bytes < 199)
					break;
			}
			return respond;
		}
	}
	return respond;
}
int IsDirectory(std::string path)
{
	if(access(path.c_str(), F_OK) == 0)
	{
		DIR *directory =  opendir(path.c_str());
		if(directory == NULL)
			return 0;
		closedir(directory);
		return 1;
	}
	return -1;
	
}
String	ServerRun::ParssingRecuistContent(String	ContentRequist)
{
	_requset					requist;
	std::string					port;
	String						serverName;
	std::vector<String>			spletLines;
	std::vector<ServerModel>	Serv;

	spletLines 	= ParssingRequist::SplitBynewLine(ContentRequist);
	requist 	= ParssingRequist::setreq(spletLines);
	serverName	= requist.header["Host"].size() != 1 ? "" : requist.header["Host"][0].split(':')[0];
	if(serverName.empty())
		return "<h1> Invalid URL </h1>";

	/**
		* @details check the if the Host have a port, if not you can work with port 80
	*/

	port = requist.header["Host"][0].split(':').size() == 2 ? requist.header["Host"][0].split(':')[1] : "80";
	Serv = servers.getServersByPort((unsigned short)strtol(port.c_str(), NULL, 10));

	/**
		* @details check the if the Host is exist
	*/
	for (size_t index_Serv = 0; index_Serv < Serv.size(); index_Serv++)
	{
		std::vector<Data>	_data;

		_data = Serv[index_Serv].getData("server_name");
		for (size_t index_data = 0; index_data < _data.size(); index_data++)
		{
			std::vector<String>	serversname;

			serversname = String(_data[index_data].getValue()).split();
			for (size_t index_serversname = 0; index_serversname < serversname.size(); index_serversname++)
			{
				if(serversname[index_serversname] == serverName)
				{
					String				path;
					path = Serv[index_Serv].getData("root").size() ? std::string(Serv[index_Serv].getData("root")[0].getValue()).append(requist.requistLine[1]) : "";
					if(path.empty())
						path.append("/");
					Location loca = ServerModel::getLocationByPath(Serv[index_Serv].getLocation(), requist.requistLine[1]);
					if (loca.getPath().empty() == false)
					{
						if(!loca.getData("root").size())
							path = std::string(loca.getData("root")[0].getValue()).append(requist.requistLine[1]);
						if (IsDirectory(path) == 1)
							return (getrespond(loca, ""));
						else
							return ERROR_404;
					}
					else
					{
						std::cout << path << std::endl;
						if (IsDirectory(path) == 0)
							return(getrespond(loca, path));
						else if(IsDirectory(path) == 1)
							return(getrespond(loca, path));
						else
							return ERROR_404;
					}
				}
			}
		}
	}
	
	return ERROR_404;
}

void erase(std::vector<struct pollfd> & struct_fdsS, size_t j)
{
	std::vector<struct pollfd> returnFds;
	for (size_t i = 0; i < struct_fdsS.size(); i++)
	{
		if(i != j)
			returnFds.push_back(struct_fdsS[i]);
	}
	struct_fdsS = returnFds;
}

std::string status(std::string statuscode, std::string stringcode)
{
	(void) statuscode;
	(void) stringcode;
	std::string header = "HTTP/1.1 ";
	header.append(statuscode);
	header.append("\r\n\r\n");
	header.append(stringcode);
	return header;
}

void	ServerRun::HandelRequist(struct pollfd	*struct_fds ,size_t	i, std::vector<struct pollfd>	&struct_fdsS, std::vector<int> servers)
{
	if (struct_fds[i].revents & POLLOUT || struct_fds[i].revents &  POLLIN)
	{
		for (size_t j = 0; j < servers.size(); j++)
		{
			if(struct_fds[i].fd == servers[j])
			{
				int			newfd;
				newfd = accept(struct_fds[i].fd, (struct sockaddr *)NULL, NULL);
				if( newfd < 0 )
				{
					std::cout << "	error	:	accpet	" << std::endl;	
					exit(0);
				}
				fcntl(newfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
				struct pollfd fd;
				fd.fd = newfd;
				fd.events = POLLOUT | POLLIN;
				struct_fdsS.push_back(fd);
				return;
			}
		}
		ssize_t		bytes;
		std::string	ContentRequist;
		char		req[2];
		bytes = 0;
		ContentRequist.clear();
		bzero(req, 2);
		while((bytes = recv(struct_fds[i].fd, req, 1, 0)) > 0)
		{
			ContentRequist.append(req);
			bzero(req, 2);
		}
		if(bytes < 0 && !ContentRequist.size())
			return;
		std::cout << ContentRequist << std::endl;
		std::string respond = status("200 OK", ParssingRecuistContent(ContentRequist));
		// std::cout << respond << std::endl;
		write(struct_fds[i].fd, respond.c_str(), respond.length());
		close(struct_fds[i].fd);
		erase(struct_fdsS,i);
	}
}

void	ServerRun::acceptRquist( std::vector<int>	servers ) 
{
	std::vector<struct pollfd>	struct_fdsS;
	struct pollfd fd;
	int pollValeu;
	for (size_t	i = 0; i < servers.size(); i++)
	{
		fd.fd = servers[i];
		fd.events = POLLOUT | POLLIN;
		struct_fdsS.push_back(fd);
	}
	while (1)
	{
		std::vector<struct pollfd>	struct_fdsC(struct_fdsS);
		pollValeu = poll(&struct_fdsC[0], (unsigned int)struct_fdsC.size(), 6000);
		if (pollValeu < 0)
		{
			std::cout << "error : poll" << std::endl;
			exit(1);
		}
		if (pollValeu == 0)
		{
			std::cout << "RELOAD SERVER" << std::endl;
			continue ;
		}
		for(size_t i = 0; i < struct_fdsC.size(); i++)
			HandelRequist(&struct_fdsC[0], i, struct_fdsS, servers);
	}
}
void	ServerRun::RunAllServers()
{
	std::vector<int> ports;
	std::vector<ServerModel> smodel = servers.getAllServers();
	std::vector<int> serversSocket;
	for(size_t i = 0; i < smodel.size(); i++)
	{
		std::vector<Data> portsValeu = smodel[i].getData("listen");
		for(size_t j = 0; j < portsValeu.size(); j++)
		{
			ports.push_back((int)strtol(portsValeu[j].getValue().c_str(), NULL, 10));
		}
	}
	for(size_t i = 0; i < ports.size(); i++)
	{
		int serverfd = this->Newsocket();
		if(this->bindConection(ports[i], serverfd))
		{
			std::cout << "PORT : " << ports[i] << " can't bind" << std::endl;
			continue;
		}
		std::cout << "PORT : " << ports[i] << " binded" << std::endl;
		this->listenSocket(serverfd);
		serversSocket.push_back(serverfd);
	}
	if(!serversSocket.size())
	{
		int serverfd = this->Newsocket();
		this->bindConection(80, serverfd);
		this->listenSocket(serverfd);
		serversSocket.push_back(serverfd);
	}
	this->acceptRquist( serversSocket );
}

bool	ServerRun::bindConection(int port, int socketfd)
{
	struct sockaddr_in addressSocketStruct;
	bzero(&addressSocketStruct, sizeof(addressSocketStruct));
	addressSocketStruct.sin_family = PF_INET;
	addressSocketStruct.sin_port = htons(port);
	addressSocketStruct.sin_addr.s_addr = INADDR_ANY;

	if(bind(socketfd,	(struct sockaddr *)&addressSocketStruct, sizeof(addressSocketStruct)) == 0)
		return 0;
	close(socketfd);
	return 1;
}
void ServerRun::listenSocket(int socketfd)
{
	int lestenr = listen(socketfd, 10);
	if(lestenr < 0)
	{
		std::cout << "error : listen" << std::endl;
		exit(0);
	}
}
ServerRun::~ServerRun()
{
	//close(socketfd);
}
