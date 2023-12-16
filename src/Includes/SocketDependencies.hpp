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

#define DEFAULT 0
#define PUTINFILE 1
#define PUTINSTRING 2

class   SocketDependencies
{
	std::string	request;
	std::string	boundary;
	std::string	filename;
	ssize_t		lenght;
	ssize_t		contenlenght;
	int			fd;
	int 		method;
	pollfd		fdpoll;

	public:

		SocketDependencies();
		~SocketDependencies();
		// SocketDependencies(const SocketDependencies&);

		// SocketDependencies &operator=(const SocketDependencies&);
	
		void			setFdPoll(const pollfd &);
		void			setFileName(const std::string &);
		void			setFdPoll(const int &, const short &);
		void			setFdPoll(const short &);
		void			setRequist(const std::string &);
		void 			setMethod(const int &);
		void			setLenght(const size_t &);
		void			setContenlenght(const ssize_t &);
		void			setBoundary(const std::string	&);
		void			setFd(const int &);

		const std::string&	getFileName() const;
		const std::string&	getRequist() const;
		const std::string&	getBoundary() const;
		const pollfd&		getFdPoll() const;
		const ssize_t &		getLenght() const;
		const ssize_t &		getContenlenght() const;
		const int &			getMethod() const;
		const int &			getFd() const;

		void		appendLenght(const size_t &);
		bool		ischunked;
		int			status;
		std::string	respond;
    	std::string chunked_rest;
		std::string	rest;
};

#endif