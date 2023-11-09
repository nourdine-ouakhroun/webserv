#ifndef PARSSINGREQUIST_HPP
#define PARSSINGREQUIST_HPP
#include"server.hpp"

class ParssingRequist
{
	private :
		string RequistContent;
		map<string, string> Requist;
	public :
	ParssingRequist();
	void TakeRequistContent(string requist);
	void TakeRequistContent(string requist);
	~ParssingRequist();
};
void acceptRquist( std::vector<int> servers );
#endif
