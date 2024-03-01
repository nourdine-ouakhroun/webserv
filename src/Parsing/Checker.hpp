#ifndef CHECKER_HPP
#define CHECKER_HPP

#include "Patterns/ServerPattern.hpp"

class Checker
{
    vector<ServerPattern> servers;
    Checker( void );
    public:
        Checker(const vector<ServerPattern>& servers);
        Checker(const Checker& copy);
        ~Checker( void );
        Checker&    operator=(const Checker& target);
        void        fullCheck( void );
        void        checkLocation(const vector<LocationPattern>& loca, String key, String oppositeKey = "unknown");
        void        checkValues( void );
        void        checkLocationValues(const vector<LocationPattern>& loca);
        void        checkLocationValues(const vector<LocationPattern>& loca, String key);
        void        checkBooleanValues(String key);
        void        checkDuplicate(String key, String oppositeKey = "unknown");
        // void        checkLocationClientMaxBodySize(const vector<LocationPattern>& loca);

        // void        checkClientMaxBodySize();
        void	CheckClientBodySize();
        void    checkLocationClientMaxBodySize(const vector<LocationPattern>& loca);

};


#endif