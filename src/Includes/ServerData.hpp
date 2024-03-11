#ifndef	SERVERDATA_HPP
#define	SERVERDATA_HPP

#include "Parser.hpp"
#include "ServerException.hpp"
#include <algorithm>

class ServerData
{
	private :
		vector<ServerPattern> servers;

	public :
		ServerData( void );
		ServerData(const vector<ServerPattern>&);
		ServerData(const ServerData&);
		ServerData&	operator=(const ServerData&);
		void	setServerData(const vector<ServerPattern>&);
		void	displayServers( void );
		static vector<ServerPattern>	getServersByServerName(const vector<ServerPattern>& servers, const String& serverName);
		static vector<ServerPattern>	getServersByIpAndPort(const vector<ServerPattern>& servers, String strHost);
		static ServerPattern			getDefaultServer(const vector<ServerPattern>& servers);
		const vector<ServerPattern>&	getAllServers() const;

		static vector<ServerPattern>	getServer(const ServerData& servers, const String& ClientAddress, String strHost);

};

#endif