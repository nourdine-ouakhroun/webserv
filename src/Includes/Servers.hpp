/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nouakhro <nouakhro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 16:41:52 by nouakhro          #+#    #+#             */
/*   Updated: 2023/12/11 17:42:09 by nouakhro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERS_HPP
#define SERVERS_HPP
#include"ReadRequest.hpp"
#include<poll.h>
#include<fstream>
#include<chrono>

typedef struct sockaddr_in S_address;

class	Servers
{
	vector<int>		fdSockets;
	ServerData		servers;
	vector<Socket>	master;
	// vector<Socket>	working;

public:

	Servers(ServerData	srvers);
	~Servers();

	void			setRespond(const string &, size_t);
	// void			setWorkingSockets(const vector<Socket> &);
	void			setMasterSockets();
	string			&getRespond(size_t);
	vector<int>		getAllPorts(void) const;
	const Socket	&getSocket(size_t) const ;
	const string 	&getHeader(size_t) const ;

	void	runAllServers(void);
	void	initSockets(vector<int> &);
	void	initSocketPort80(void);
	void	readyToRead(size_t);
	void	readyToWrite(size_t);
	void	erase(size_t );
	size_t	SocketsSize(void) const ;
	short	Revents(size_t) const ;
	void	acceptConection(size_t);

	void 	isSocketsAreReady(vector<pollfd> &poll_fd);
	const vector<Socket>&	getMasterSockets(void) const ;

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