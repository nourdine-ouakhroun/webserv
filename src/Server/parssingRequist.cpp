
#include"ParssingRequist.hpp"
#include"ServerData.hpp"
ParssingRequist::ParssingRequist( void )
{
	
}
ParssingRequist::ParssingRequist( std::string RequistContent )
{
	this->RequistContent = RequistContent;
}
Data ParssingRequist::SpliteEvryLine( std::string LineRequist )
{
	size_t Position;
	Data _data;
	Position = LineRequist.find(':');
	if(Position != SIZE_T_MAX)
	{
		_data.setKey(LineRequist.substr(0, Position));
		_data.setValue(LineRequist.substr(Position + 1));
	}
	else
		throw std::runtime_error("");

	return(_data);
}
std::vector<std::string>	ParssingRequist::SplitBynewLine(  )
{
	std::vector<std::string>	Content;
	std::string					tockens;
	std::istringstream			sstr(RequistContent);

	while(getline(sstr, tockens, '\n'))
	{
		tockens[tockens.size() -1] = 0;
			Content.push_back(tockens);
	}
	return Content;
}
ParssingRequist::~ParssingRequist()
{
	
}
