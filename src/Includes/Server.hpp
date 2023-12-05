#ifndef	SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <unistd.h>
#include "String.hpp"
#include "Poll.hpp"
#include "ServerException.hpp"
#include "ServerData.hpp"
#include <fcntl.h>

class Server
{
	struct	sockaddr_in	socketData;
	socklen_t	socketLen;
	unsigned short	port;
	static const int errorNumber = -202;


	public :
		Poll	fds;
		Server( void );
		Server(const unsigned short &_port);
		Server(const Server& copy);
		~Server( void ) throw();
		Server&	operator=(const Server& target);
		int		createNewSocket(unsigned short port);
		int		getAvailabeFD( void );
		int		accept(int targetSocket);
		String	recieve(int socket);
		ssize_t	send(int socket, String response);
		Poll	getFds( void ) const
		{
			return (fds);
		};
};

String	readFile(const String& path);
unsigned short	getPort(String	value);
std::vector<ServerPattern>	getServer(ServerData& servers, std::vector<Data> header);
std::vector<int>	openAllPorts(const std::vector<ServerPattern>& serversInfo, Server& server);


#endif
