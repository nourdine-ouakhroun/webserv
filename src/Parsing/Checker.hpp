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
        void        checkDuplicate(String key);
        void        checkLocation(const vector<LocationPattern>& loca, String key);
        void        checkValues( void );
        void        checkLocationValues(const vector<LocationPattern>& loca);
        void        checkLocationValues(const vector<LocationPattern>& loca, String key);
        void        checkBooleanValues(String key);

};


#endif