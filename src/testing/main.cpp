#include <iostream>
#include <vector>

std::vector<std::string>* getStrings( void )
{
	std::vector<std::string>* vec = new std::vector<std::string>;
	std::string str = "hello world";
	vec->push_back(str);
	return (vec);
}

int	main( void )
{
	std::vector<std::string>* vec = getStrings();
	std::cout << *vec->begin() << std::endl;
	return (0);
}
