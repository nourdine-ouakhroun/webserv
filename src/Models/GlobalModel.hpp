#ifndef GLOBAL_MODEL
#define GLOBAL_MODEL

#include <exception>
#include "Data.hpp"


class GlobalModel
{
	std::vector<Data> data;

	public	:
		GlobalModel( void );
		GlobalModel(const GlobalModel& copy);
		~GlobalModel( void );
		GlobalModel&	operator=(const GlobalModel& target);
		void	addData(const Data& _data);
		const Data&	getData(const String& key);
		const std::vector<Data>&	getAllData( void ) const;


};

#endif