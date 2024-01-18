
#include"ParssingRequest.hpp"
#include"ServerData.hpp"
ParssingRequest::ParssingRequest( void ){

}

Data ParssingRequest::SpliteEvryLine( String LineRequest)
{
	size_t Position;
	Data _data;
	Position = LineRequest.find(": ");
	if(Position != SIZE_T_MAX)
	{
		_data.setKey(LineRequest.substr(0, Position));
		_data.setValue(LineRequest.substr(Position + 2, LineRequest.size() - (Position + 2) - 1));
	}
	else
		throw runtime_error("");

	return(_data);
}
_requset ParssingRequest::setreq( vector<String> requses )
{
	_requset _req;

	_req.RequestLine = requses[0].split();
	// _req.RequestLine[1].rightTrim("/");
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
vector<String>	ParssingRequest::SplitBynewLine( String RequestContent )
{
	vector<String>	Content;
	String					tockens;
	istringstream			sstr(RequestContent);

	while(getline(sstr, tockens, '\n'))
	{
		tockens[tockens.size() -1] = 0;
		Content.push_back(tockens);
	}
	return Content;
}
ParssingRequest::~ParssingRequest()
{
	
}
