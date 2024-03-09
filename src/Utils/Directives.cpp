#include "Directives.hpp"

Directives::Directives(const GeneralPattern &_data, const String& _path = "") : data(_data), path(_path)
{
}

Directives::~Directives( void )
{
}

void	Directives::handleLogges( void )
{
	static int stdout_fd;
	static int stderr_fd;

	vector<Data> accessLog = data.getData("access_log");
	vector<Data> errorLog = data.getData("error_log");
	if (accessLog.empty() && errorLog.empty())
		return ;
	/**
	 * @attention check fd leaks here.
	*/
	if (!stdout_fd && (stdout_fd = dup(STDOUT_FILENO)) == -1)
	{
		Logger::error(cerr , "dup() function failed.", "");
		return ;
	}
	if (!stderr_fd && (stderr_fd = dup(STDERR_FILENO)) == -1)
	{
		Logger::error(cerr , "dup() function failed.", "");
		return ;
	}
	if (dup2(stdout_fd, STDOUT_FILENO) == -1 || dup2(stderr_fd, STDERR_FILENO) == -1)
	{
		Logger::error(cerr , "dup2() function failed.", "");
		return ;
	}

	if (accessLog.empty() == false)
	{
		int file_fd = open(accessLog.at(0).getValue().c_str() , O_CREAT | O_APPEND | O_RDWR, 0777);
		if (file_fd == -1 || dup2(file_fd, STDOUT_FILENO) == -1 || close(file_fd) == -1)
		{
			close(file_fd);
			Logger::error(cerr , "failed to open access_log file : ", accessLog.at(0).getValue().c_str());
			return ;
		}
		close(file_fd);
	}
	if (errorLog.empty() == false)
	{
		int file_fd = open(errorLog.at(0).getValue().c_str() , O_CREAT | O_APPEND | O_RDWR, 0777);
		if (file_fd == -1 || dup2(file_fd, STDERR_FILENO) == -1)
		{
			close(file_fd);
			Logger::error(cerr , "failed to open error_log file : ", errorLog.at(0).getValue().c_str());
			return ;
		}
		close(file_fd);
	}
}
