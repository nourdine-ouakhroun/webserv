#include "Checker.hpp"
#include "ParsingException.hpp"

Checker::Checker( void )
{
}

Checker::~Checker( void )
{
    servers.clear();
}

Checker::Checker(const vector<ServerPattern>& _servers) : servers(_servers)
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

void    Checker::checkLocation(const vector<LocationPattern>& loca, String key, String oppositeKey)
{
    if (loca.empty() == true)
        return ;
    for (size_t i = 0; i < loca.size(); i++)
    {
        if (loca.at(i).getData("listen").empty() == false 
            || loca.at(i).getData("server_name").empty() == false)
            throw (ParsingException("Check Faild in " + key));
        
        vector<Data> data = loca.at(i).getData(key);
        vector<Data> oppositeData = loca.at(i).getData(oppositeKey);
        if (data.size() > 1 || (data.size() && oppositeData.size()))
            throw (ParsingException("Check Faild in " + key));
        if (loca.at(i).getInnerLocation().empty() == false)
            checkLocation(loca.at(i).getInnerLocation(), key);  
    }
}

void    Checker::checkLocationValues(const vector<LocationPattern>& loca)
{
    if (loca.empty() == true)
        return ;
    for (size_t i = 0; i < loca.size(); i++)
    {
        vector<Data> data = loca.at(i).getAllData();
        if (data.empty() == false)
            for (size_t j = 0; j < data.size(); j++)
                if (data.at(j).getValue().empty() == true)
                    throw (ParsingException(data.at(j).getKey().append(" directive is duplicate.")));
        if (loca.at(i).getInnerLocation().empty() == false)
            checkLocationValues(loca.at(i).getInnerLocation());  
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
                    throw (ParsingException(data.at(j).getKey().append(" directive is duplicate.")));
        checkLocationValues(servers.at(i).getLocations());
    }
}

void    Checker::checkDuplicate(String key, String oppositeKey)
{
    for (size_t i = 0; i < servers.size(); i++)
    {
        vector<Data> data = servers.at(i).getData(key);
        vector<Data> oppositeData = servers.at(i).getData(oppositeKey);
        if (data.size() > 1 || (data.size() && oppositeData.size()))
            throw (ParsingException("Check Faild in " + key));
        checkLocation(servers.at(i).getLocations(), key, oppositeKey);
    }
}


void    Checker::checkLocationValues(const vector<LocationPattern>& loca, String key)
{
    if (loca.empty() == true)
        return ;
    static vector<String> booleanValues;
    if (booleanValues.empty())
    {
        booleanValues.push_back("on");
        booleanValues.push_back("off");
    }
    for (size_t i = 0; i < loca.size(); i++)
    {
        vector<Data> data = loca.at(i).getData(key);
        if (data.size() > 1 || (data.size() == 1
            && find(booleanValues.begin(), booleanValues.end(), data.at(0).getValue()) == booleanValues.end()))
            throw (ParsingException("Check Faild in " + key));
        if (loca.at(i).getInnerLocation().empty() == false)
            checkLocationValues(loca.at(i).getInnerLocation(), key);  
    }
}

void    Checker::checkBooleanValues(String key)
{
    static vector<String> booleanValues;
    if (booleanValues.empty())
    {
        booleanValues.push_back("on");
        booleanValues.push_back("off");
    }
    for (size_t i = 0; i < servers.size(); i++)
    {
        vector<Data> data = servers.at(i).getData(key);
        if (data.size() > 1 || (data.size() == 1
            && find(booleanValues.begin(), booleanValues.end(), data.at(0).getValue()) == booleanValues.end()))
            throw (ParsingException("Check Faild in " + key));
        checkLocationValues(servers.at(i).getLocations(), key);
    }
}

bool    checkClientBodySizeValue(const String& value)
{
    static String numbers(".0123456789");
    static String characters("GgMmKk");
    size_t count = 0;
    for (size_t i = 0; i < value.size(); i++)
    {
        size_t numPos = numbers.find(value[i]);
        size_t charPos = characters.find(value[i]);
        if (numPos == String::npos && charPos == String::npos)
            return (false);
        if (charPos != String::npos)
            count++;
    }
    if (count != 1)
        return (false);
    return (true);
}

void    Checker::checkLocationClientMaxBodySize(const vector<LocationPattern>& loca)
{
    if (loca.empty() == true)
        return ;
    for (size_t i = 0; i < loca.size(); i++)
    {
        vector<Data> data = loca.at(i).getData("client_max_body_size");        
        if (data.size() > 1 || !checkClientBodySizeValue(data.front().getValue()))
            throw (ParsingException("checking Faild in `client_max_body_size`."));
        if (loca.at(i).getInnerLocation().empty() == false)
            checkLocationClientMaxBodySize(loca.at(i).getInnerLocation());  
    }
}

void	Checker::CheckClientBodySize()
{
    for (size_t i = 0; i < servers.size(); i++)
    {
        vector<Data> data = servers.at(i).getData("client_max_body_size");
        if (data.size() > 1 || (!data.empty() && !checkClientBodySizeValue(data.front().getValue())))
            throw (ParsingException("checking Faild in `client_max_body_size`."));
        checkLocationClientMaxBodySize(servers.at(i).getLocations());
    }
}


void    Checker::fullCheck( void )
{
    if (servers.empty() == true)
        return ;
    checkValues();
    checkDuplicate("root", "alias");
    checkDuplicate("alias", "root");
    checkDuplicate("return");
    checkDuplicate("try_files");
    checkDuplicate("autoindex");
    checkDuplicate("error_log");
    checkDuplicate("access_log");
    checkBooleanValues("autoindex");
    CheckClientBodySize();
}