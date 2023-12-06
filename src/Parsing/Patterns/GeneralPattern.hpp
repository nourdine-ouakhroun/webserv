#ifndef GENERALPATTERN_HPP
#define GENERALPATTERN_HPP

#include <exception>
#include "../Data.hpp"

class GeneralPattern
{
	std::vector<Data> data;

	public	:
		GeneralPattern( void );
		GeneralPattern(std::vector<Data> _data);
		GeneralPattern(const GeneralPattern& copy);
		virtual						~GeneralPattern( void ) throw();
		GeneralPattern&			operator=(const GeneralPattern& target);

		void						addData(const Data& _data);
		std::vector<Data>			getData(const String& key) const;
		const std::vector<Data>&	getAllData( void ) const;
		bool						isExist(const Data& value);
		virtual bool				empty( void ) const;
		virtual	String				getPath( void ) const;
		virtual	void				execute( void ) const;
		static	void				printGeneralPattern(const GeneralPattern&, String&);
};

#endif
