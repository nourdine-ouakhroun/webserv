#include"FileDepandenc.hpp"


// FileDepandenc::FileDepandenc(/* args */)
// {
// 	std::cout << "default" << std::endl;
// }

// FileDepandenc::~FileDepandenc()
// {
// 	std::cout << "destractor" << std::endl;
// }

const std::string &FileDepandenc::getRequist() const 
{
	return requist;
}
const int &FileDepandenc::getStatus() const
{
	return status;
}

void FileDepandenc::setRequist(std::string requist) 
{
	this->requist.append(requist);
}
void FileDepandenc::setStatus(const int &status)
{
	this->status = status;
}