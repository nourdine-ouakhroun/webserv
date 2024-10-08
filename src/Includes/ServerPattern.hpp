#ifndef SERVER_MODEL
#define SERVER_MODEL

#include <unistd.h>
#include <dirent.h>
#include <algorithm>
#include <map>
#include "LocationPattern.hpp"

String	readFile(const String& path);

class	ServerPattern : public GeneralPattern
{
	vector<LocationPattern>	location;
	public :
		map<string, string>		mimeTypes;
		
		ServerPattern( void );
		ServerPattern(const GeneralPattern&, const vector<LocationPattern>&);
		ServerPattern& operator=(const ServerPattern& target);
		ServerPattern(const ServerPattern& copy);
		~ServerPattern( void )	throw();
		void			setLocation(vector<LocationPattern>& _location);
		void			addLocation(LocationPattern _location);
		static			LocationPattern	getLocationByPath(vector<LocationPattern> locations, const String& srcPath);
		const			vector<LocationPattern>&	getLocations( void ) const;

		void			addDirectiveToLocation(vector<LocationPattern>&	servers, const String& key, const vector<Data>& serverRoot, const String &oppositeKey = "unknown");
		void			addDirectives(const String& key, const String &oppositeKey = "unknown");

		bool			empty( void ) const;
		String			getPath( void ) const;
		void			addRootsDirectivesToNestedLocation();

		static int		checkIsDirectory(const String& filename);
		static	void	printServerPatternInfo(const ServerPattern& serverPattern);
		static void		getAllLocationPath(const vector<LocationPattern>& lcts, vector<String> &paths);
};


#endif
