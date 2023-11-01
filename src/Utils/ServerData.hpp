#ifndef	SERVER_DATA
#define	SERVER_DATA

#include "../Models/ServerModel.hpp"

class ServerData
{
	std::vector<ServerModel> data;
	public :
		ServerData(const std::vector<ServerModel>&);
		ServerData(const ServerData&);
		ServerData&	operator=(const ServerData&);
		void	displayData( void );
		const std::vector<ServerModel>&	getData( void ) const;
		const std::vector<Location*>&	getServerLocations( void ) const;
};

#endif
