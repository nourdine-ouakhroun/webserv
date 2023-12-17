/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeroual <mzeroual@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 16:41:52 by nouakhro          #+#    #+#             */
/*   Updated: 2023/12/12 17:19:36 by mzeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include"ServerData.hpp"
#include"Parser.hpp"
#include"ServerModel.hpp"
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
	public:
	~Server();

	static void	bindSocket(int, int);
	static int	setSocket();
	static void	listenPort(int);
};

#endif