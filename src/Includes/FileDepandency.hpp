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
class   FileDependency
{
private:

	std::string	requist;
	std::string	boundary;
	size_t		lenght;
	size_t		contenlenght;
	int			fd;
	int 		method;
	pollfd		fdpoll;

public:

	void setFdPoll(int fd, short events);
	void setRequist(std::string requist, size_t lenght);
	void setFdPoll(pollfd &poll_struct);
	pollfd &getFdPoll() const;
	FileDependency();
	~FileDependency();

	std::string	rest;
};

#endif