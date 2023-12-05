#include "ServerPattern.hpp"

ServerPattern::ServerPattern( void ) : GeneralPattern()
{
}

ServerPattern::ServerPattern(const GeneralPattern& model, const std::vector<LocationPattern>& _location) : GeneralPattern(model), location(_location)
{
	addDirectives("root");
	addDirectives("error_page");
	addDirectives("error_log");
	addDirectives("access_log");
	addDirectives("Options");
	addDirectives("AddHandler");
	addDirectives("autoindex");
}

void	ServerPattern::addDirectives(const String& key)
{
	std::vector<Data> data = getData(key);
	if (data.empty() == false)
		addDirectiveToLocation(location, key, data.at(0).getValue());
}

ServerPattern::~ServerPattern( void ) throw()
{
	location.clear();
}

ServerPattern::ServerPattern(const ServerPattern& copy) : GeneralPattern(copy)
{
	*this = copy;
}

ServerPattern& ServerPattern::operator=(const ServerPattern& target)
{
	if (this != &target)
	{
		GeneralPattern::operator=(target);
		location = target.location;
		addDirectives("Options");
		addDirectives("error_page");
		addDirectives("error_log");
		addDirectives("access_log");
		addDirectives("root");
		addDirectives("AddHandler");
		addDirectives("autoindex");
	}
	return (*this);
}

void	ServerPattern::setLocation(std::vector<LocationPattern>& _location)
{
	location = _location;
	addDirectives("root");
	addDirectives("error_page");
	addDirectives("error_log");
	addDirectives("access_log");
	addDirectives("Options");
	addDirectives("AddHandler");
	addDirectives("autoindex");
}

const std::vector<LocationPattern>&	ServerPattern::getLocation( void ) const
{
	return (location);
}

void	ServerPattern::addLocation(LocationPattern _location)
{
	location.push_back(_location);
}

void	ServerPattern::printServerPatternInfo(const ServerPattern& serverPattern)
{
	String s("\t");
	GeneralPattern::printGeneralPattern(serverPattern, s);
	std::cout << s << ">>>> Location Info <<<<\n";
	LocationPattern::printAllLocations(serverPattern.getLocation(), s);
}

LocationPattern	ServerPattern::getLocationByPath(std::vector<LocationPattern> locations, const String& srcPath)
{
	static std::vector<LocationPattern> tmplocations;
	if (tmplocations.empty() == true)
		tmplocations = locations;
	std::vector<LocationPattern>::iterator ibegin = locations.begin();
	std::vector<LocationPattern>::iterator iend = locations.end();
	while (ibegin < iend)
	{
		String tmpPath(ibegin->getPath());
		if (!srcPath.compare(tmpPath) && tmpPath.length() == srcPath.length())
			return (*ibegin);
		if (ibegin->getInnerLocation().empty() == false)
		{
			LocationPattern loca = getLocationByPath(ibegin->getInnerLocation(), srcPath);
			if (loca.getPath().empty() == false)
				return (loca);
		}
		ibegin++;
	}
	return (LocationPattern());
}

void	ServerPattern::addDirectiveToLocation(std::vector<LocationPattern>&	servers, const String& key, const String& serverDirective)
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
		std::vector<LocationPattern>& innerLocation = servers.at(i).getInnerLocation();
		if (innerLocation.empty() == false)
			addDirectiveToLocation(innerLocation, key, rootValue);
	}
}

int		ServerPattern::checkIsDirectory(const String& filename)
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

bool	ServerPattern::empty( void ) const
{
	if (GeneralPattern::empty() && location.empty())
		return (true);
	return (false);
}

void	ServerPattern::execute( void ) const
{
	std::cout << "HELLO WORLD" << std::endl;
}