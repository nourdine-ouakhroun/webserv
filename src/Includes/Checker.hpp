#ifndef CHECKER_HPP
#define CHECKER_HPP

#include "ServerModel.hpp"

class Checker
{
    vector<ServerModel> servers;
    Checker( void );
    public:
        Checker(const vector<ServerModel>& servers);
        Checker(const Checker& copy);
        ~Checker( void );
        Checker&    operator=(const Checker& target);
        void        fullCheck( void );
        void        checkDuplicate(String key);
        void        checkLocation(const vector<Location>& loca, String key);
        void        checkValues( void );
        void        checkLocationValues(const vector<Location>& loca);


};


#endif