#include "String.hpp"

String::String( void ) : std::string()
{
}

String::String(const char* str) : std::string(str)
{
}

String::String(const String& str) : std::string(str)
{
}

String::~String( void )
{
}

String& String::operator=(const String& str)
{
	std::string::operator=(str);
	return (*this);
}


String&	String::leftTrim(const String& characters)
{
	iterator iterBegin = begin();
	iterator iterEnd = end();

	while (iterBegin < iterEnd)
	{
		if (characters.find(*iterBegin) != npos)
			iterBegin++;
		else
			break ;
	}
	erase(begin(), iterBegin);
	return (*this);
}

String&	String::rightTrim(const String& characters)
{
	iterator iterBegin = begin();
	iterator iterEnd = end();

	while (iterBegin < --iterEnd)
	{
		if (characters.find(*iterEnd) != npos)
			erase(iterEnd);
		else
			break ;
	}
	return (*this);
}


String&	String::trim(const String& characters)
{
	leftTrim(characters);
	rightTrim(characters);
	return (*this);
}

std::vector<String> String::split(const char& separator)
{
	std::vector<String> vec;
	std::istringstream str(*this);
    String s;
    while (getline(str, s, separator))
	{
		if (s.size() == 0)
			continue ;
        vec.push_back(s);
    }
	return (vec);
}

int	String::countRepeating(const char& c)
{
	int	counter = 0;
	iterator iBeg = begin();
	iterator iEnd = end();
	while (iBeg < iEnd)
	{
		if (*iBeg == c)
			counter++;
		iBeg++;
	}
	return (counter);
}
