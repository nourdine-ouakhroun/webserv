/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nouakhro <nouakhro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 16:41:52 by nouakhro          #+#    #+#             */
/*   Updated: 2023/11/28 16:39:38 by nouakhro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include"ServerData.hpp"
#include"Parser.hpp"
#include"webserver.h"
#include<sys/socket.h>
#include <netinet/in.h>
#ifndef BACKLOG
#define BACKLOG 10
#endif
typedef struct sockaddr_in S_address;
class	Server
{
	private : 
	Server();
	~Server();
	public:

	static void	bindSocket(int, int);
	static int		setSocket();
	static void	listenPort(int);
};

#endif
