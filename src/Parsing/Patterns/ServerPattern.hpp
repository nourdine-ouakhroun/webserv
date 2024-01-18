#ifndef SERVER_MODEL
#define SERVER_MODEL

#include <unistd.h>
#include <dirent.h>
#include <algorithm>
#include "LocationPattern.hpp"

String	readFile(const String& path);

class	ServerPattern : public GeneralPattern
{
	vector<LocationPattern>	location;
	public :
		ServerPattern( void );
		ServerPattern(const GeneralPattern&, const vector<LocationPattern>&);
		ServerPattern& operator=(const ServerPattern& target);
		ServerPattern(const ServerPattern& copy);
		~ServerPattern( void )	throw();
		void	setLocation(vector<LocationPattern>& _location);
		void	addLocation(LocationPattern _location);
		static	LocationPattern	getLocationByPath(vector<LocationPattern> locations, const String& srcPath);
		const	vector<LocationPattern>&	getLocations( void ) const;
		void	addDirectiveToLocation(vector<LocationPattern>&	servers, const String& key, const String& serverRoot);
		void	addDirectives(const String& key);
		static int	checkIsDirectory(const String& filename);

		static	void	printServerPatternInfo(const ServerPattern& serverPattern);

		bool			empty( void ) const;
		void	execute( void ) const;
		static void    getAllLocationPath(const vector<LocationPattern>& lcts, vector<String> &paths);

};


#endif
