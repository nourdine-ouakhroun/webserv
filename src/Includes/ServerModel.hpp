#ifndef SERVER_MODEL
#define SERVER_MODEL

#include "Location.hpp"
#include "GlobalModel.hpp"
#include <unistd.h>
#include <dirent.h>
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

		bool	checkIsDirectory(String filename)
		{
			if (!access(filename.c_str(), F_OK))
			{
				DIR *dir = opendir(filename.c_str());
				if (dir != NULL)
				{
					closedir(dir);
					return (true);
				}
			}
			return (false);
		}

		template <typename T>
		static bool	findLocationByPath(std::vector<Location> locations, const String& rootPath,
								const String& srcPath, void (*to_do) (const Location&, T&), T& value)
		{
			std::vector<Location>::iterator ibegin = locations.begin();
			std::vector<Location>::iterator iend = locations.end();
			while (ibegin < iend)
			{
				std::vector<Data> data = ibegin->getData("root");
				if (data.empty() == true)
					ibegin->addData(Data("root", rootPath));
				String tmpPath;
				tmpPath.append(ibegin->getPath());
				std::vector<Data> aliases = ibegin->getData("alias");
				Logger::info(std::cout, "srcPath : ", srcPath);
				Logger::info(std::cout, "tmpPath : ", tmpPath);
				for (size_t i = 0; i < aliases.size(); i++)
				{
					std::vector<String> values = String(aliases[i].getValue()).split();
					if (std::find(values.begin(), values.end(), srcPath) != values.end())
						return (to_do(*ibegin, value), true);
				}
				if (!srcPath.compare(tmpPath.trim(" \t\n\r")) && tmpPath.length() == srcPath.length())
					return (to_do(*ibegin, value), true);
				if (ibegin->getInnerLocation().empty() == false \
					&& findLocationByPath(ibegin->getInnerLocation(), ibegin->getData("root").at(0).getValue(), srcPath, to_do, value) == true)
					return (true);
				ibegin++;
			}
			return (false);
		}

		static	void	printServerModelInfo(const ServerModel& serverModel);

};

#endif
