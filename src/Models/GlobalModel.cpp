#include "GlobalModel.hpp"

GlobalModel::GlobalModel( void )
{

}

GlobalModel::GlobalModel(vector<Data> _data) : data(_data)
{
}

GlobalModel::GlobalModel(const GlobalModel& copy)
{
	*this = copy;
}


GlobalModel::~GlobalModel( void )
{
}

GlobalModel&	GlobalModel::operator=(const GlobalModel& target)
{
	if (this != &target)
	{
		data = target.data;
	}
	return (*this);
}

void	GlobalModel::addData(const Data& _data)
{
	data.push_back(_data);
}

vector<Data>	GlobalModel::getData(const String& key) const
{
	vector<Data>	returnData;
	vector<Data>::const_iterator	iterBegin = data.begin();
	vector<Data>::const_iterator	iterEnd = data.end();
	while (iterBegin < iterEnd)
	{
		if (!iterBegin->getKey().compare(key))
			returnData.push_back(*iterBegin);
		iterBegin++;
	}
	return (returnData);
}

const vector<Data>&	GlobalModel::getAllData( void ) const
{
	return (data);
}



bool	GlobalModel::isExist(const Data& value)
{
	vector<Data>::const_iterator	iterBegin = data.begin();
	vector<Data>::const_iterator	iterEnd = data.end();
	while (iterBegin < iterEnd)
	{
		if (!iterBegin->getKey().compare(value.getKey()) \
			&& !iterBegin->getValue().compare(value.getValue()))
			return (true);
		iterBegin++;
	}
	return (false);
}



void	GlobalModel::printGlobalModel(const GlobalModel& model, String &str)
{
	vector<Data>::const_iterator	ibegin = model.getAllData().begin();
	vector<Data>::const_iterator	iend = model.getAllData().end();
	while (ibegin < iend)
	{
		Data::printData(*ibegin, str);
		ibegin++;
	}
}
