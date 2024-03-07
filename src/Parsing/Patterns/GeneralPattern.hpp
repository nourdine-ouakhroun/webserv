#ifndef GENERALPATTERN_HPP
#define GENERALPATTERN_HPP

#include <exception>
#include "../Data.hpp"

class GeneralPattern
{
	vector<Data> data;

	public	:
		GeneralPattern( void );
		GeneralPattern(vector<Data> _data);
		GeneralPattern(const GeneralPattern& copy);
		virtual						~GeneralPattern( void ) throw();
		GeneralPattern&				operator=(const GeneralPattern& target);

		GeneralPattern&				setData(const vector<Data> &data);
		void						updateData(const Data& _data, size_t pos);
		void						addData(const Data& _data);
		vector<Data>			getData(const String& key) const;
		const vector<Data>&	getAllData( void ) const;
		bool						isExist(const Data& value);
		virtual bool				empty( void ) const;
		virtual	String				getPath( void ) const;
		virtual	void				execute( void ) const;
		static	void				printGeneralPattern(const GeneralPattern&, const String&);
		static	void				printGeneralPattern(const GeneralPattern&, String&);
};

#endif
