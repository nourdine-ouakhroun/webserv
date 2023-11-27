/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nouakhro <nouakhro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 16:41:52 by nouakhro          #+#    #+#             */
/*   Updated: 2023/11/27 17:22:45 by nouakhro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include"ServerData.hpp"
#include"Parser.hpp"
#include"ServerModel.hpp"
#include<sys/socket.h>
class	Server
{
private:
	ServerData	servers;
	std::vector<int> fdports;
public:
	Server( ServerData );
	~Server();

	void	runAllServers();
	void	setSocket(int);
};

#endif