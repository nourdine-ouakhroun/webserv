#include "Checker.hpp"
#include "ParsingException.hpp"

Checker::Checker( void )
{
}

Checker::~Checker( void )
{
    servers.clear();
}

Checker::Checker(const std::vector<ServerPattern>& _servers) : servers(_servers)
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

void    Checker::checkLocation(const std::vector<LocationPattern>& loca, String key)
{
    if (loca.empty() == true)
        return ;
    for (size_t i = 0; i < loca.size(); i++)
    {
        if (loca.at(i).getData("listen").empty() == false 
            || loca.at(i).getData("server_name").empty() == false)
            throw (ParsingException("Check Faild."));
        
        std::vector<Data> data = loca.at(i).getData(key);
        if (data.size() > 1)
            throw (ParsingException("Check Faild."));
        if (loca.at(i).getInnerLocation().empty() == false)
            checkLocation(loca.at(i).getInnerLocation(), key);  
    }
}

void    Checker::checkLocationValues(const std::vector<LocationPattern>& loca)
{
    if (loca.empty() == true)
        return ;
    for (size_t i = 0; i < loca.size(); i++)
    {
        std::vector<Data> data = loca.at(i).getAllData();
        if (data.empty() == false)
            for (size_t j = 0; j < data.size(); j++)
                if (data.at(j).getValue().empty() == true)
                    throw (ParsingException(data.at(j).getKey().append(" directive is duplicate.")));
        if (loca.at(i).getInnerLocation().empty() == false)
            checkLocationValues(loca.at(i).getInnerLocation());  
    }
}

void    Checker::checkDuplicate(String key)
{
    for (size_t i = 0; i < servers.size(); i++)
    {
        std::vector<Data> data = servers.at(i).getData(key);
        if (data.size() > 1)
            throw (ParsingException("Check Faild."));
        checkLocation(servers.at(i).getLocation(), key);
    }
}

void    Checker::checkValues( void )
{
    for (size_t i = 0; i < servers.size(); i++)
    {
        std::vector<Data> listens = servers.at(i).getData("listen");
        if (listens.empty() == true)
            servers[i].addData(Data("lsten", "80"));
        std::vector<Data> data = servers.at(i).getAllData();
        if (data.empty() == false)
            for (size_t j = 0; j < data.size(); j++)
                if (data.at(j).getValue().empty() == true)
                    throw (ParsingException(data.at(j).getKey().append(" directive is duplicate.")));
        checkLocationValues(servers.at(i).getLocation());
    }
}

void    Checker::checkLocationValues(const std::vector<LocationPattern>& loca, String key)
{
    if (loca.empty() == true)
        return ;
    std::vector<String> booleanValues;
    booleanValues.push_back("on");
    booleanValues.push_back("off");
    for (size_t i = 0; i < loca.size(); i++)
    {
        std::vector<Data> data = loca.at(i).getData(key);
        if (data.size() > 1 || (data.size() == 1
            && std::find(booleanValues.begin(), booleanValues.end(), data.at(0).getValue()) == booleanValues.end()))
            throw (ParsingException("Check Faild."));
        if (loca.at(i).getInnerLocation().empty() == false)
            checkLocationValues(loca.at(i).getInnerLocation(), key);  
    }
}

void    Checker::checkBooleanValues(String key)
{
    for (size_t i = 0; i < servers.size(); i++)
    {
        std::vector<Data> data = servers.at(i).getData(key);
        if (data.size() > 1)
            throw (ParsingException("Check Faild."));
        checkLocationValues(servers.at(i).getLocation(), key);
    }
}

void    Checker::fullCheck( void )
{
    if (servers.empty() == true)
        return ;
    checkValues();
    checkDuplicate("root");
    checkDuplicate("return");
    checkDuplicate("alias");
    checkDuplicate("try_files");
    checkDuplicate("autoindex");
    checkDuplicate("error_log");
    checkDuplicate("access_log");
    checkBooleanValues("autoindex");
}