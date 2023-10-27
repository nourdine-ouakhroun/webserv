#ifndef	DATA_HPP
#define DATA_HPP

#include <iostream>

class Data
{
	std::string	key;
	std::string value;

	public :
		Data(const std::string& _key, const std::string& _value);
		void	setKey(const std::string& Key);
		void	setValue(const std::string& Value);
		const std::string&	getKey( void ) const;
		const std::string&	getValue( void ) const;

};

#endif
