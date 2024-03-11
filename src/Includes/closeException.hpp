#ifndef	CLOSEEXCEPTION_HPP
# define CLOSEEXCEPTION_HPP

#include <exception>
#include "String.hpp"

class closeException : public exception
{
	String	message;

	public :
		closeException( void );
		virtual ~closeException( void ) throw();
		virtual const char*	what( void ) const throw();
};

#endif

