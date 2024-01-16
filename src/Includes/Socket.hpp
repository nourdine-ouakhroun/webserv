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
using namespace std;
#define DEFAULT 0
#define PUTINFILE 1
#define PUTINSTRING 2

class   Socket
{
	string	header;
	string	boundary;
	ssize_t content_lenght;
	string	body;
	int 		method;
	pollfd		fdpoll;

	public:

		Socket();
		~Socket();

		void	setFdPoll(const pollfd &);
		void	setFdPoll(const int &, const short &);
		void	setFdPoll(const short &);
		void	setHeader(const string &);
		void	setMethod(const int &);
		void	setBody(const 	string &);
		void	setBoundary(const string &	boundary);
		void	setContenlenght(const ssize_t & contenlenght);

		string & getBodyChange();
		const string	&getBoundary() const;
		const ssize_t	&getContenlenght() const;
		const string 	&getBody() const;
		const string	&getHeader() const;
		const pollfd	&getFdPoll() const;
		const int		&getMethod() const;

		size_t	is_chuncked;
		string	respond;
		size_t	hex_valeu;
};

#endif