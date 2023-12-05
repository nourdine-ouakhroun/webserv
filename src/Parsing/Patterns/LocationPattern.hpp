#ifndef	LOCATIONPATTERN_HPP
# define LOCATIONPATTERN_HPP

#include <iostream>
#include "GeneralPattern.hpp"

class	LocationPattern : public GeneralPattern
{
	std::vector<LocationPattern>	innerLocation;
	String		path;

	public :
		LocationPattern( void );
		LocationPattern(const GeneralPattern&, const String&, const std::vector<LocationPattern>&);
		LocationPattern(const LocationPattern& copy);
		~LocationPattern( void ) throw();
		LocationPattern& operator=(const LocationPattern& target);
		void	setPath(const String& Path);
		const std::vector<LocationPattern>&	getInnerLocation( void ) const;
		std::vector<LocationPattern>&	getInnerLocation( void );
		static	void	printAllLocations(const std::vector<LocationPattern>&, String&);
		
		bool			empty( void ) const;
		String			getPath( void ) const;
		void			execute( void ) const;
};

#endif
