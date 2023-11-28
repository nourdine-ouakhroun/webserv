/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManageServers.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nouakhro <nouakhro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 16:41:52 by nouakhro          #+#    #+#             */
/*   Updated: 2023/11/28 23:10:08 by nouakhro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANAGESERVERS
#define MANAGESERVERS
#include"Server.hpp"
#include<poll.h>
typedef struct sockaddr_in S_address;
class	ManageServers
{
private:
	ServerData	servers;
	std::vector<int> fdSockets;
public:
	ManageServers(ServerData	srvers);
	~ManageServers();

	void	runAllServers();
	void	acceptConection();
	void	handler(std::vector<pollfd>&, std::vector<pollfd> &, size_t);
};

#endif