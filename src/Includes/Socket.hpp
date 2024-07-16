/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeroual <mzeroual@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 19:51:28 by nouakhro          #+#    #+#             */
/*   Updated: 2024/01/19 21:35:51 by mzeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEDEPANDENCY_HPP
#define FILEDEPANDENCY_HPP
#include<iostream>
#include<poll.h>
#include<algorithm>
#include "webserver.h"
#include"closeException.hpp"
using namespace std;

class   Socket
{
	string	request;
	string	boundary;
	size_t content_lenght;
	pollfd		fdpoll;

	public:
		String		ipAndPort;

		Socket();
		~Socket();

		void	setFdPoll(const pollfd &);
		void	setFdPoll(const int &, const short &);
		void	setFdPoll(const short &);
		void	setRequest(const 	string &);
		void	setBoundary(const string &	boundary);
		void	setContenlenght(const size_t & contenlenght);

		string & changeRequest();
		const string	&getBoundary() const;
		const size_t	&getContenlenght() const;
		const string 	&getRequest() const;
		const pollfd	&getFdPoll() const;
		const int		&getMethod() const;

		bool 	first_read;
		size_t	is_chuncked;
		string	respond;
		size_t	hex_valeu;
};

#endif