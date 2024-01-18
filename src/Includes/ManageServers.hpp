/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManageServers.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeroual <mzeroual@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 16:41:52 by nouakhro          #+#    #+#             */
/*   Updated: 2024/01/16 17:44:07 by mzeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANAGESERVERS
#define MANAGESERVERS

#include <fstream>
#include"Server.hpp"
#include"FileDepandenc.hpp"
#include<poll.h>
#include<fstream>
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
	void	handler(std::vector<FileDepandenc>&, std::vector<FileDepandenc> &, size_t);
};

#endif