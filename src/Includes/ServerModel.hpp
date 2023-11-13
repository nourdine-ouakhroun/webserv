#ifndef SERVER_MODEL
#define SERVER_MODEL

#include "Location.hpp"
#include "GlobalModel.hpp"

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
			bool	isDone = false;
			while (ibegin < iend)
			{
				String tmpPath(destPath);
				tmpPath.append(ibegin->getPath());
				if (!srcPath.compare(tmpPath) && tmpPath.length() == srcPath.length())
				{
					to_do(*ibegin, value);
					isDone = true;
				}
				if (isDone)
					return (true);
				if (ibegin->getInnerLocation().empty() == false && findLocationByPath(ibegin->getInnerLocation(), tmpPath, srcPath, to_do, value) == true)
					return (true);
				ibegin++;
			}
			return (false);
		}

		static	void	printServerModelInfo(const ServerModel& serverModel);
		
};

#endif

