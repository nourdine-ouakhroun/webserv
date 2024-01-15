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
	vector<Location>	location;

	public :
		ServerModel( void );
		ServerModel(const GlobalModel&, const vector<Location>&);
		ServerModel& operator=(const ServerModel& target);
		ServerModel(const ServerModel& copy);
		~ServerModel( void )	throw();
		void	setLocation(vector<Location>& _location);
		void	addLocation(Location _location);
		static	Location	getLocationByPath(vector<Location> locations, const String& srcPath);
		const vector<Location>&	getLocation( void ) const;
		void	addRootToLocation(vector<Location>&	servers, const String& serverRoot);

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
		static bool	findLocationByPath(vector<Location> locations, const String& rootPath,
								const String& srcPath, void (*to_do) (const Location&, T&), T& value);

		static	void	printServerModelInfo(const ServerModel& serverModel);

};

#include "../Templates/ServerModel.tpp"

#endif
