#include "Data.hpp"

Data::Data(const std::string& _key, const std::string& _value) : key(_key), value(_value)
{
}

void	Data::setKey(const std::string& Key)
{
	key = Key;
}

void	Data::setValue(const std::string& Value)
{
	value = Value;
}

const std::string& Data::getKey( void ) const
{
	return (key);
}

const std::string& Data::getValue( void ) const
{
	return (value);
}


