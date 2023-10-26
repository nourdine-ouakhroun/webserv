#include "GlobalModel.hpp"

GlobalModel::GlobalModel( void )
{

}

GlobalModel::GlobalModel(
		std::string _listen,
		std::string _root,
		std::string _index,
		std::string _allowMethods,
		std::string _errorPage) :
	listen(_listen),
	root(_root),
	index(_index),
	allowMethods(_allowMethods),
	errorPage(_errorPage)
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
		listen = target.listen;
		root = target.root;
		index = target.index;
		allowMethods = target.allowMethods;
		errorPage = target.errorPage;
	}
	return (*this);
}

void	GlobalModel::setListen(std::string Listen)
{
	listen = Listen;
}


void	GlobalModel::setRoot(std::string Root)
{
	root = Root;
}

void	GlobalModel::setIndex(std::string Index)
{
	index = Index;
}


void	GlobalModel::setAllowMethods(std::string AllowMethods)
{
	allowMethods = AllowMethods;
}


void	GlobalModel::setErrorPage(std::string ErrorPage)
{
	errorPage = ErrorPage;
}

std::string	GlobalModel::getListen( void ) const 
{
	return (this->listen);
}

std::string	GlobalModel::getIndex( void ) const 
{
	return (this->index);
}

std::string	GlobalModel::getRoot( void ) const 
{
	return (this->root);
}

std::string	GlobalModel::getAllowMethods( void ) const 
{
	return (this->allowMethods);
}


std::string	GlobalModel::getErrorPage( void ) const 
{
	return (this->errorPage);
}


