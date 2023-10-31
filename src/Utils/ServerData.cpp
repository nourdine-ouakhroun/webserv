#include "ServerData.hpp"

ServerData::ServerData(const std::vector<ServerModel>& _data) : data(_data)
{
}

ServerData::ServerData(const ServerData& copy)
{
	*this = copy;
}

ServerData&	ServerData::operator=(const ServerData& target)
{
	data = target.data;
	return (*this);
}

void	ServerData::displayData( void )
{

}

const std::vector<ServerModel>&	ServerData::getData( void ) const
{
	return (data);
}

const std::vector<Location*>&	ServerData::getServerLocations( void ) const
{
	return (data.begin()->getLocation());
}
