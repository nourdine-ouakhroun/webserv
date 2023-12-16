/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManageServers.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nouakhro <nouakhro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 16:41:52 by nouakhro          #+#    #+#             */
/*   Updated: 2023/12/11 17:42:09 by nouakhro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANAGESERVERS
#define MANAGESERVERS
#include"Server.hpp"
#include"FileDependency.hpp"
#include<poll.h>
#include<fstream>
#include<chrono>

#define NPOS  std::string::npos 
#define GET 0
#define POST 1
#define DELET 2

#ifndef READ_NUMBER
#define READ_NUMBER 2025
#endif
typedef struct sockaddr_in S_address;
class	ManageServers
{
private:
	ServerData	servers;
	std::vector<int> fdSockets;
	std::vector<FileDependency> master;
	std::vector<FileDependency> working;
public:

	ManageServers(ServerData	srvers);
	~ManageServers();

	void					setRespond(const std::string & respond, size_t index);
	void					setWorkingSockets(const std::vector<FileDependency> &);
	std::string&			getRespond(size_t index);
	std::vector<int>		getAllPorts() const;
	const FileDependency&	getWorkingSocket(size_t index) const ;
	const std::string& 		getRequest(size_t) const ;

	void	runAllServers(void);
	void	initSockets(std::vector<int> &);
	void	initSocketPort80(void);
	void	acceptConection(void);
	void	readyToRead(size_t);
	void	readyToWrite(size_t);
	void	erase(size_t j);
	size_t	WorkingSocketsSize() const ;
	short	WorkingRevents(size_t) const ;


	std::vector<FileDependency>			isSocketsAreReady();
	const std::vector<FileDependency>&	getMasterSockets() const ;

	class PollException : std::exception
	{
		std::string exception_msg;

		public :
			PollException();
			PollException(const std::string &);
			const char * what() const throw();
			~PollException() throw();
	};

};

#endif