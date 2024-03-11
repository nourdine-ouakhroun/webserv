#ifndef CHECKER_HPP
#define CHECKER_HPP

#include "ServerPattern.hpp"

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

        void        CheckClientBodySize();
        void        checkLocationClientMaxBodySize(const vector<LocationPattern>& loca);
        void        checkServers(String key);
        void        checkDuplicateListen( void );
        void        checkCgiTime( void );




};


#endif