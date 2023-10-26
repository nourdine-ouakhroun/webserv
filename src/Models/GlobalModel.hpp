#ifndef GLOBAL_MODEL
#define GLOBAL_MODEL

#include <iostream>
#include <vector>

class GlobalModel
{
	std::string	listen;
	std::string	root;
	std::string	index;
	std::string	allowMethods;
	std::string	errorPage;

	public	:
		GlobalModel( void );
		GlobalModel(
				std::string listen,
				std::string root,
				std::string index,
				std::string allowMethods,
				std::string erorrpPage);
		GlobalModel(const GlobalModel& copy);
		~GlobalModel( void );
		GlobalModel&	operator=(const GlobalModel& target);
		void	setListen(std::string Listen);
		void	setRoot(std::string Root);
		void	setIndex(std::string Index);
		void	setAllowMethods(std::string AllowMethods);
		void	setErrorPage(std::string ErrorPage);
		std::string	getListen( void ) const;
		std::string	getRoot( void ) const;
		std::string	getIndex( void ) const;
		std::string	getAllowMethods( void ) const;
		std::string	getErrorPage( void ) const;

};

#endif
