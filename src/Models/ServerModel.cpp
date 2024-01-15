#include "ServerModel.hpp"

ServerModel::ServerModel( void ) : GlobalModel()
{
}

ServerModel::ServerModel(const GlobalModel& model, const vector<Location>& _location) : GlobalModel(model), location(_location)
{
	vector<Data> serverRoot = getData("root");
	if (serverRoot.empty() == false)
		addRootToLocation(location, serverRoot.at(0).getValue());
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
		vector<Data> serverRoot = getData("root");
		if (serverRoot.empty() == false)
			addRootToLocation(location, serverRoot.at(0).getValue());
	}
	return (*this);
}

void	ServerModel::setLocation(vector<Location>& _location)
{
	location = _location;
	vector<Data> serverRoot = getData("root");
	if (serverRoot.empty() == false)
		addRootToLocation(location, serverRoot.at(0).getValue());
}

const vector<Location>&	ServerModel::getLocation( void ) const
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
	cout << s << ">>>> Location Info <<<<\n";
	Location::printAllLocations(serverModel.getLocation(), s);
}

Location	ServerModel::getLocationByPath(vector<Location> locations, const String& srcPath)
{
	static vector<Location> tmplocations;
	if (tmplocations.empty() == true)
		tmplocations = locations;
	vector<Location>::iterator ibegin = locations.begin();
	vector<Location>::iterator iend = locations.end();
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

void	ServerModel::addRootToLocation(vector<Location>&	servers, const String& serverRoot)
{
	if (servers.empty() == true)
		return ;
	for (size_t i = 0; i < servers.size(); i++)
	{
		String rootValue(serverRoot);
		vector<Data> roots = servers.at(i).getData("root");
		if (roots.empty() == true)
			servers.at(i).addData(Data("root", rootValue));
		else
			rootValue = roots.at(0).getValue();
		vector<Location>& innerLocation = servers.at(i).getInnerLocation();
		if (innerLocation.empty() == false)
			addRootToLocation(innerLocation, rootValue);
	}
}