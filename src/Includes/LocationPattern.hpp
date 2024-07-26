#ifndef	LOCATIONPATTERN_HPP
# define LOCATIONPATTERN_HPP


#include "GeneralPattern.hpp"

class	LocationPattern : public GeneralPattern
{
	vector<LocationPattern>	innerLocation;

	public :
		LocationPattern( void );
		LocationPattern(const GeneralPattern&, const String&, const vector<LocationPattern>&);
		LocationPattern(const LocationPattern& copy);
		~LocationPattern( void ) throw();
		LocationPattern& operator=(const LocationPattern& target);
		void	setPath(const String& Path);
		const vector<LocationPattern>&	getInnerLocation( void ) const;
		vector<LocationPattern>&	getInnerLocation( void );
		static	void	printAllLocations(const vector<LocationPattern>&, String&);
		
		bool			empty( void ) const;
		String			getPath( void ) const;
};

#endif
