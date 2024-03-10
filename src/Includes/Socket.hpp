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
#include<cstdio>
#include<algorithm>
#include "webserver.h"
using namespace std;

class   Socket
{
	string	body;
	string	boundary;
	size_t content_lenght;
	int 		method;
	pollfd		fdpoll;

	public:
		String		ipAndPort;

		Socket();
		~Socket();

		void	setFdPoll(const pollfd &);
		void	setFdPoll(const int &, const short &);
		void	setFdPoll(const short &);
		void	setMethod(const int &);
		void	setBody(const 	string &);
		void	setBoundary(const string &	boundary);
		void	setContenlenght(const size_t & contenlenght);

		string & changeBody();
		const string	&getBoundary() const;
		const size_t	&getContenlenght() const;
		const string 	&getBody() const;
		const pollfd	&getFdPoll() const;
		const int		&getMethod() const;

		size_t	is_chuncked;
		string	respond;
		string request;
		size_t hex_valeu;
};

#endif