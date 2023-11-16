
#include"ParssingRequist.hpp"
#include"ServerData.hpp"
ParssingRequist::ParssingRequist( void ){

}
ParssingRequist::ParssingRequist( String requistLine ){
	this->requistLine = requistLine;
}

Data ParssingRequist::SpliteEvryLine( String LineRequist)
{
	size_t Position;
	Data _data;
	Position = LineRequist.find(": ");
	if(Position != SIZE_T_MAX)
	{
		_data.setKey(LineRequist.substr(0, Position));
		_data.setValue(LineRequist.substr(Position + 1));
	}
	else
		throw std::runtime_error("");

	return(_data);
}
void ParssingRequist::setreq( std::vector<String> requses )
{
	for (size_t i = 1; i < requses.size(); i++)
	{
		Data _data(SpliteEvryLine(requses[i]));
		String valeu = _data.getValue();
		requsteContent[_data.getKey()] = valeu.split(',');
	}
	std::cout << requsteContent["host"][0] << std::endl;
}
std::vector<String>	ParssingRequist::SplitBynewLine( String RequistContent )
{
	std::vector<String>	Content;
	String					tockens;
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
