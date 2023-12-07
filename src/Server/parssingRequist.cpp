
#include"ParssingRequist.hpp"
#include"ServerData.hpp"
ParssingRequist::ParssingRequist( void ){

}

Data ParssingRequist::SpliteEvryLine( String LineRequist)
{
	size_t Position;
	Data _data;
	Position = LineRequist.find(": ");
	if(Position != SIZE_T_MAX)
	{
		_data.setKey(LineRequist.substr(0, Position));
		_data.setValue(LineRequist.substr(Position + 2, LineRequist.size() - (Position + 2) - 1));
	}
	else
		throw std::runtime_error("");

	return(_data);
}
_requset ParssingRequist::setreq( std::vector<String> requses )
{
	_requset _req;

	_req.requistLine = requses[0].split();
	// _req.requistLine[1].rightTrim("/");
	size_t i;
	for (i = 1; i < requses.size(); i++)
	{
		Data _data;
		try{
			_data = SpliteEvryLine(requses[i]);
		}
		catch(...){
			break;
		};
		String valeu = _data.getValue();
		_req.header[_data.getKey()] = valeu.split(',');
	}
	if(++i != requses.size())
	{
		for (; i < requses.size(); i++)
		{
			_req.body.push_back(requses[i]);
		}
	}
	return _req;
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
