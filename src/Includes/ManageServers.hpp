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
	vector<SocketDependencies>	master;
	vector<SocketDependencies>	working;
	vector<int>			fdSockets;
	ServerData					servers;

	// add All Ports to this class.
	vector<String>			allport;
public:

	ManageServers(ServerData	srvers);
	~ManageServers();

	void					setRespond(const string &, size_t);
	void					setWorkingSockets(const vector<SocketDependencies> &);
	void					setMasterSockets();
	string&			getRespond(size_t);
	vector<String>		getAllPorts(void) const;
	const SocketDependencies&	getWorkingSocket(size_t) const ;
	const string& 		getRequest(size_t) const ;

	void	runAllServers(void);
	void	initSockets(vector<String> &);
	void	initSocketPort80(void);
	// void	acceptConection(void);
	SocketDependencies	readyToRead(size_t);
	void	readyToWrite(size_t);
	void	erase(size_t );
	size_t	WorkingSocketsSize(void) const ;
	short	WorkingRevents(size_t) const ;
	void	acceptConection(size_t);


	vector<SocketDependencies>			isSocketsAreReady(void);
	const vector<SocketDependencies>&	getMasterSockets(void) const ;

	class PollException : exception
	{
		string exception_msg;

		public :
			PollException();
			PollException(const string &);
			const char * what() const throw();
			~PollException() throw();
	};

};

#endif