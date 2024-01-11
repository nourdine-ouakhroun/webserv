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
#include"ReadRequest.hpp"
#include<poll.h>
#include<fstream>
#include<chrono>

typedef struct sockaddr_in S_address;

class	ManageServers
{
	std::vector<SocketDependencies>	master;
	std::vector<SocketDependencies>	working;
	std::vector<int>			fdSockets;
	ServerData					servers;

	// add All Ports to this class.
	std::vector<String>			allport;
public:

	ManageServers(ServerData	srvers);
	~ManageServers();

	void					setRespond(const std::string &, size_t);
	void					setWorkingSockets(const std::vector<SocketDependencies> &);
	void					setMasterSockets();
	std::string&			getRespond(size_t);
	std::vector<String>		getAllPorts(void) const;
	const SocketDependencies&	getWorkingSocket(size_t) const ;
	const std::string& 		getRequest(size_t) const ;

	void	runAllServers(void);
	void	initSockets(std::vector<String> &);
	void	initSocketPort80(void);
	// void	acceptConection(void);
	void	readyToRead(size_t);
	void	readyToWrite(size_t);
	void	erase(size_t );
	size_t	WorkingSocketsSize(void) const ;
	short	WorkingRevents(size_t) const ;
	void	acceptConection(size_t);


	std::vector<SocketDependencies>			isSocketsAreReady(void);
	const std::vector<SocketDependencies>&	getMasterSockets(void) const ;

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