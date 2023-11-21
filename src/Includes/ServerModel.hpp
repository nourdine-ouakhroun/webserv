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

		static int	checkIsDirectory(const String& filename);

		static	void	printServerModelInfo(const ServerModel& serverModel);

};

// #include "../Templates/ServerModel.tpp"

#endif
