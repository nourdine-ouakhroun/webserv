#ifndef	LOCATION_HPP
# define LOCATION_HPP

#include <iostream>
#include "GlobalModel.hpp"

class	Location : public GlobalModel
{
	vector<Location>	innerLocation;
	String		path;

	public :
		Location( void );
		Location(const GlobalModel&, const String&, const vector<Location>&);
		Location(const Location& copy);
		~Location( void );
		Location& operator=(const Location& target);
		void	setPath(const String& Path);
		const String&	getPath( void ) const;
		const vector<Location>&	getInnerLocation( void ) const;
		vector<Location>&	getInnerLocation( void );
		static	void	printAllLocations(const vector<Location>&, String&);
};

#endif
