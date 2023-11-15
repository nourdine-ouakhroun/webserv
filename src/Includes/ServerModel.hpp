#ifndef SERVER_MODEL
#define SERVER_MODEL

#include "Location.hpp"
#include "GlobalModel.hpp"
#include <algorithm>

class	ServerModel : public GlobalModel
{
	std::vector<Location>	location;

	public :
		ServerModel( void );
		ServerModel(const GlobalModel&, const std::vector<Location>&);
		ServerModel& operator=(const ServerModel& target);
		ServerModel(const ServerModel& copy);
		~ServerModel( void )	throw();
		void	setLocation(std::vector<Location>& _location);
		void	addLocation(Location _location);

		const std::vector<Location>&	getLocation( void ) const;

		template <typename T>
		static bool	findLocationByPath(
								const std::vector<Location>& locations,
								String& destPath, const String& srcPath,
								void (*to_do) (const Location&, T&), T& value)
		{
			std::vector<Location>::const_iterator ibegin = locations.begin();
			std::vector<Location>::const_iterator iend = locations.end();
			while (ibegin < iend)
			{
				String tmpPath(destPath);
				tmpPath.append(ibegin->getPath());
				std::vector<Data> aliases = ibegin->getData("alias");
				for (size_t i = 0; i < aliases.size(); i++)
				{
					std::vector<String> values = String(aliases[i].getValue()).split();
					if (std::find(values.begin(), values.end(), srcPath) != values.end())
						return (to_do(*ibegin, value), true);
				}
				String	src(srcPath);
				size_t	pos = srcPath.find_last_of('.');
				if (pos != String::npos)
				{
					size_t pos2 = srcPath.find_last_of('/');
					if (pos2 != String::npos && pos2 < pos)
						src = srcPath.substr(0, pos2);
				}
				if (!src.compare(tmpPath) && tmpPath.length() == src.length())
					return (to_do(*ibegin, value), true);
				if (ibegin->getInnerLocation().empty() == false \
					&& findLocationByPath(ibegin->getInnerLocation(), tmpPath, src, to_do, value) == true)
					return (true);
				ibegin++;
			}
			return (false);
		}

		static	void	printServerModelInfo(const ServerModel& serverModel);
		
};

#endif

