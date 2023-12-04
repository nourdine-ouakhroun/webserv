
#ifndef GETMETHOD_HPP
#define GETMETHOD_HPP

// #include <iostream>
#include "AMethods.hpp"

typedef std::map<std::string, std::string> maps;

class GetMethod : public AMethods
{
    private:
	
    public:
        GetMethod( void );
        ~GetMethod( void );
		GetMethod( std::string request );

		void parseBody( void );
        // void parseRequestLine( void );
        // void parseHeader( void );
        // void parseBody() const = 0;
};


#endif