#ifndef GLOBALMODEL_HPP
#define GLOBALMODEL_HPP

#include <exception>
#include "Data.hpp"

class GlobalModel
{
	vector<Data> data;

	public	:
		GlobalModel( void );
		GlobalModel(vector<Data> _data);
		GlobalModel(const GlobalModel& copy);
		virtual						~GlobalModel( void );
		GlobalModel&				operator=(const GlobalModel& target);
		void						addData(const Data& _data);
		vector<Data>			getData(const String& key) const;
		const vector<Data>&	getAllData( void ) const;
		bool						isExist(const Data& value);
		static	void				printGlobalModel(const GlobalModel&, String&);
};

#endif
