#ifndef SERVER_MODEL
#define SERVER_MODEL

#include <unistd.h>
#include <dirent.h>
#include <algorithm>
#include "Location.hpp"
#include "GlobalModel.hpp"

String	readFile(const String& path);
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
		static	Location	getLocationByPath(std::vector<Location> locations, const String& srcPath);
		const std::vector<Location>&	getLocation( void ) const;
		void	addDirectiveToLocation(std::vector<Location>&	servers, const String& key, const String& serverRoot);
		void	addDirectives(const String& key);

		static int	checkIsDirectory(String filename)
		{
			if (!access(filename.c_str(), F_OK))
			{
				DIR *dir = opendir(filename.c_str());
				if (dir != NULL)
				{
					closedir(dir);
					return (1);
				}
				return (0);
			}
			return (-1);
		}

		template <typename T>
		static bool	findLocationByPath(std::vector<Location> locations, const String& rootPath,
								const String& srcPath, void (*to_do) (const Location&, T&), T& value);

		static	void	printServerModelInfo(const ServerModel& serverModel);

};

#include "../Templates/ServerModel.tpp"

#endif
