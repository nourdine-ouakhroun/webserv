#ifndef	SERVER_DATA
#define	SERVER_DATA

#include "../Models/ServerModel.hpp"

class ServerData
{
	std::vector<ServerModel> data;
	public :
		ServerData(const std::vector<ServerModel>&);
		ServerData(const ServerData&);
		
};

#endif
