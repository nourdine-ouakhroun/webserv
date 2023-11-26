#include "ServerModel.hpp"

ServerModel::ServerModel( void ) : GlobalModel()
{
}

ServerModel::ServerModel(const GlobalModel& model, const std::vector<Location>& _location) : GlobalModel(model), location(_location)
{
	addDirectives("root");
	addDirectives("autoindex");
}

void	ServerModel::addDirectives(const String& key)
{
	std::vector<Data> data = getData(key);
	if (data.empty() == false)
		addDirectiveToLocation(location, key, data.at(0).getValue());
}

ServerModel::~ServerModel( void ) throw()
{
	location.clear();
}

ServerModel::ServerModel(const ServerModel& copy) : GlobalModel(copy)
{
	*this = copy;
}

ServerModel& ServerModel::operator=(const ServerModel& target)
{
	if (this != &target)
	{
		GlobalModel::operator=(target);
		location = target.location;
		addDirectives("root");
		addDirectives("autoindex");
	}
	return (*this);
}

void	ServerModel::setLocation(std::vector<Location>& _location)
{
	location = _location;
	addDirectives("root");
	addDirectives("autoindex");
}

const std::vector<Location>&	ServerModel::getLocation( void ) const
{
	return (location);
}

void	ServerModel::addLocation(Location _location)
{
	location.push_back(_location);
}

void	ServerModel::printServerModelInfo(const ServerModel& serverModel)
{
	String s("\t");
	GlobalModel::printGlobalModel(serverModel, s);
	std::cout << s << ">>>> Location Info <<<<\n";
	Location::printAllLocations(serverModel.getLocation(), s);
}

Location	ServerModel::getLocationByPath(std::vector<Location> locations, const String& srcPath)
{
	static std::vector<Location> tmplocations;
	if (tmplocations.empty() == true)
		tmplocations = locations;
	std::vector<Location>::iterator ibegin = locations.begin();
	std::vector<Location>::iterator iend = locations.end();
	while (ibegin < iend)
	{
		String tmpPath(ibegin->getPath());
		if (!srcPath.compare(tmpPath) && tmpPath.length() == srcPath.length())
			return (*ibegin);
		if (ibegin->getInnerLocation().empty() == false)
		{
			Location loca = getLocationByPath(ibegin->getInnerLocation(), srcPath);
			if (loca.getPath().empty() == false)
				return (loca);
		}
		ibegin++;
	}
	return (Location());
}

void	ServerModel::addDirectiveToLocation(std::vector<Location>&	servers, const String& key, const String& serverDirective)
{
	if (servers.empty() == true)
		return ;
	for (size_t i = 0; i < servers.size(); i++)
	{
		String rootValue(serverDirective);
		std::vector<Data> roots = servers.at(i).getData(key);
		if (roots.empty() == true)
			servers.at(i).addData(Data(key, rootValue));
		else
			rootValue = roots.at(0).getValue();
		std::vector<Location>& innerLocation = servers.at(i).getInnerLocation();
		if (innerLocation.empty() == false)
			addDirectiveToLocation(innerLocation, key, rootValue);
	}
}

int		ServerModel::checkIsDirectory(const String& filename)
{
	DIR *dir;
	if (!access(filename.c_str(), F_OK))
	{
		if ((dir = opendir(filename.c_str())) != NULL)
			return (closedir(dir), 1);
		return (0);
	}
	return (-1);
}