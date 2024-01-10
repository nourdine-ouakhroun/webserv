#include "ServerPattern.hpp"

ServerPattern::ServerPattern( void ) : GlobalModel()
{
}

ServerPattern::ServerPattern(const GlobalModel& model, const std::vector<Location>& _location) : GlobalModel(model), location(_location)
{
	std::vector<Data> serverRoot = getData("root");
	if (serverRoot.empty() == false)
		addRootToLocation(location, serverRoot.at(0).getValue());
}

ServerPattern::~ServerPattern( void ) throw()
{
	location.clear();
}

ServerPattern::ServerPattern(const ServerPattern& copy) : GlobalModel(copy)
{
	*this = copy;
}

ServerPattern& ServerPattern::operator=(const ServerPattern& target)
{
	if (this != &target)
	{
		GlobalModel::operator=(target);
		location = target.location;
		std::vector<Data> serverRoot = getData("root");
		if (serverRoot.empty() == false)
			addRootToLocation(location, serverRoot.at(0).getValue());
	}
	return (*this);
}

void	ServerPattern::setLocation(std::vector<Location>& _location)
{
	location = _location;
	std::vector<Data> serverRoot = getData("root");
	if (serverRoot.empty() == false)
		addRootToLocation(location, serverRoot.at(0).getValue());
}

const std::vector<Location>&	ServerPattern::getLocation( void ) const
{
	return (location);
}

void	ServerPattern::addLocation(Location _location)
{
	location.push_back(_location);
}

void	ServerPattern::printServerPatternInfo(const ServerPattern& ServerPattern)
{
	String s("\t");
	GlobalModel::printGlobalModel(ServerPattern, s);
	std::cout << s << ">>>> Location Info <<<<\n";
	Location::printAllLocations(ServerPattern.getLocation(), s);
}

Location	ServerPattern::getLocationByPath(std::vector<Location> locations, const String& srcPath)
{
	static std::vector<Location> tmplocations;
	if (tmplocations.empty() == true)
		tmplocations = locations;
	std::vector<Location>::iterator ibegin = locations.begin();
	std::vector<Location>::iterator iend = locations.end();
	while (ibegin < iend)
	{
		String tmpPath(String(ibegin->getPath()).rightTrim("/"));
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

void	ServerPattern::addRootToLocation(std::vector<Location>&	servers, const String& serverRoot)
{
	if (servers.empty() == true)
		return ;
	for (size_t i = 0; i < servers.size(); i++)
	{
		String rootValue(serverRoot);
		std::vector<Data> roots = servers.at(i).getData("root");
		if (roots.empty() == true)
			servers.at(i).addData(Data("root", rootValue));
		else
			rootValue = roots.at(0).getValue();
		std::vector<Location>& innerLocation = servers.at(i).getInnerLocation();
		if (innerLocation.empty() == false)
			addRootToLocation(innerLocation, rootValue);
	}
}