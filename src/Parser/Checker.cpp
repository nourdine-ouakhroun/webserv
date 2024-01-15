#include "Checker.hpp"
#include "ParsingException.hpp"

Checker::Checker( void )
{
}

Checker::~Checker( void )
{
    servers.clear();
}

Checker::Checker(const vector<ServerModel>& _servers) : servers(_servers)
{
}

Checker::Checker(const Checker& copy) : servers(copy.servers)
{
}

Checker& Checker::operator=(const Checker& target)
{
    if (this != &target)
        servers = target.servers;
    return (*this);
}

void    Checker::checkLocation(const vector<Location>& loca, String key)
{
    if (loca.empty() == true)
        return ;
    for (size_t i = 0; i < loca.size(); i++)
    {
        vector<Data> data = loca.at(i).getData(key);
        if (data.size() > 1)
            throw (ParsingException("Check Faild."));
        if (loca.at(i).getInnerLocation().empty() == false)
            checkLocation(loca.at(i).getInnerLocation(), key);  
    }
}

void    Checker::checkLocationValues(const vector<Location>& loca)
{
    if (loca.empty() == true)
        return ;
    for (size_t i = 0; i < loca.size(); i++)
    {
        vector<Data> data = loca.at(i).getAllData();
        if (data.empty() == false)
            for (size_t j = 0; j < data.size(); j++)
                if (data.at(j).getValue().empty() == true)
                    throw (ParsingException(String(data.at(j).getKey()).append(" directive is duplicate.")));
        if (loca.at(i).getInnerLocation().empty() == false)
            checkLocationValues(loca.at(i).getInnerLocation());  
    }
}

void    Checker::checkDuplicate(String key)
{
    for (size_t i = 0; i < servers.size(); i++)
    {
        vector<Data> data = servers.at(i).getData(key);
        if (data.size() > 1)
            throw (ParsingException("Check Faild."));
        checkLocation(servers.at(i).getLocation(), key);
    }
}

void    Checker::checkValues( void )
{
    for (size_t i = 0; i < servers.size(); i++)
    {
        vector<Data> data = servers.at(i).getAllData();
        if (data.empty() == false)
            for (size_t j = 0; j < data.size(); j++)
                if (data.at(j).getValue().empty() == true)
                    throw (ParsingException(String(data.at(j).getKey()).append(" directive is duplicate.")));
        checkLocationValues(servers.at(i).getLocation());
    }
}

void    Checker::fullCheck( void )
{
    if (servers.empty() == true)
        return ;
    checkValues();
    checkDuplicate("root");
    checkDuplicate("try_files");
}