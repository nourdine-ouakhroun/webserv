/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDepandency.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nouakhro <nouakhro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 19:51:28 by nouakhro          #+#    #+#             */
/*   Updated: 2023/12/11 18:09:59 by nouakhro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEDEPANDENCY_HPP
#define FILEDEPANDENCY_HPP
#include<iostream>
#include<poll.h>
#include<cstdio>
#include<algorithm>
#include "webserver.h"

#define DEFAULT 0
#define PUTINFILE 1
#define PUTINSTRING 2

class   SocketDependencies
{
	string	request;
	string	boundary;
	string	filename;
	ssize_t		lenght;
	ssize_t		contenlenght;
	int			fd;
	int 		method;
	pollfd		fdpoll;

	public:

		// I add this attribute to save ip and port number.
		String		ipAndPort;

		SocketDependencies();
		~SocketDependencies();
		// SocketDependencies(const SocketDependencies& target)
		// {
		// 	*this = target;
		// }
		// SocketDependencies& operator=(const SocketDependencies& target)
		// {
		// 	if (this != &target)
		// 	{
		// 		request = target.request;
		// 		boundary = target.boundary;
		// 		filename = target.filename;
		// 		lenght = target.lenght;
		// 		contenlenght = target.contenlenght;
		// 		fd = target.fd;
		// 		method = target.method;
		// 		fdpoll = target.fdpoll;
		// 		ischunked = target.ischunked;
		// 		status = target.status;
		// 		respond = target.respond;
		// 		chunked_rest = target.chunked_rest;
		// 		rest = target.rest;
		// 		hex_valeu = target.hex_valeu;
		// 	}
		// 	return (*this);
		// }
		// SocketDependencies(const SocketDependencies&);

		// SocketDependencies &operator=(const SocketDependencies&);
		void			setFdPoll(const pollfd &);
		void			setFileName(const string &);
		void			setFdPoll(const int &, const short &);
		void			setFdPoll(const short &);
		void			setRequist(const string &);
		void 			setMethod(const int &);
		void			setLenght(const size_t &);
		void			setContenlenght(const ssize_t &);
		void			setBoundary(const string	&);
		void			setFd(const int &);

		const string&	getFileName() const;
		const string&	getRequist() const;
		const string&	getBoundary() const;
		const pollfd&		getFdPoll() const;
		const ssize_t &		getLenght() const;
		const ssize_t &		getContenlenght() const;
		const int &			getMethod() const;
		const int &			getFd() const;

		void		appendLenght(const size_t &);
		bool		ischunked;
		int			status;
		string		respond;
    	string		chunked_rest;
		string		rest;
		size_t		hex_valeu;
};

#endif