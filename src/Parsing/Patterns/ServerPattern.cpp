#include "ServerPattern.hpp"

void	ServerPattern::addRootsDirectivesToNestedLocation()
{
	addDirectives("root", "alias");
	addDirectives("index");
	addDirectives("error_page");
	addDirectives("error_log");
	addDirectives("access_log");
	addDirectives("Options");
	addDirectives("AddHandler");
	addDirectives("autoindex");
	addDirectives("cgi");
	addDirectives("client_max_body_size");
}

ServerPattern::ServerPattern( void ) : GeneralPattern()
{
	addRootsDirectivesToNestedLocation();
}

ServerPattern::ServerPattern(const GeneralPattern& model, const vector<LocationPattern>& _location) : GeneralPattern(model), location(_location)
{
	addRootsDirectivesToNestedLocation();
}


void	ServerPattern::addDirectiveToLocation(vector<LocationPattern>&	servers, const String& key, const vector<Data>& serverDirective, const String &oppositeKey)
{
	if (servers.empty() == true)
		return ;
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (!servers.at(i).getData(oppositeKey).empty())
			continue ;
		vector<Data> rootValue = serverDirective;
		vector<Data> roots = servers.at(i).getData(key);
		if (roots.empty() == true)
			for (size_t j = 0; j < rootValue.size(); j++)
				servers.at(i).addData(rootValue[j]);
		else
			rootValue = roots;
		vector<LocationPattern>& innerLocation = servers.at(i).getInnerLocation();
		if (innerLocation.empty() == false)
			addDirectiveToLocation(innerLocation, key, rootValue, oppositeKey);
	}
}

void	ServerPattern::addDirectives(const String& key, const String &oppositeKey)
{
	vector<Data> data = getData(key);
	if (data.empty() == false)
		addDirectiveToLocation(location, key, data, oppositeKey);
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
		mimeTypes = target.mimeTypes;
		addRootsDirectivesToNestedLocation();
	}
	return (*this);
}

void	ServerPattern::setLocation(vector<LocationPattern>& _location)
{
	location = _location;
	addRootsDirectivesToNestedLocation();
}

const vector<LocationPattern>&	ServerPattern::getLocations( void ) const
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
	cout << s << ">>>> Location Info <<<<\n";
	LocationPattern::printAllLocations(serverPattern.getLocations(), s);
}

LocationPattern	ServerPattern::getLocationByPath(vector<LocationPattern> locations, const String& srcPath)
{
	static vector<LocationPattern> tmplocations;
	if (tmplocations.empty() == true)
		tmplocations = locations;
	vector<LocationPattern>::iterator ibegin = locations.begin();
	vector<LocationPattern>::iterator iend = locations.end();
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
}

String	ServerPattern::getPath( void ) const
{
	return ("/");
}

void    ServerPattern::getAllLocationPath(const vector<LocationPattern>& lcts, vector<String>& paths)
{
	if (lcts.empty())
		return ;
    for (size_t i = 0; i < lcts.size(); i++)
    {
		paths.push_back(lcts[i].getPath());
		getAllLocationPath(lcts[i].getInnerLocation(), paths);
    }
}