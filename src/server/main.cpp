/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeroual <mzeroual@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 16:41:57 by nouakhro          #+#    #+#             */
/*   Updated: 2023/12/07 22:11:29 by mzeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include"ManageServers.hpp"

int main(int ac, char **av)
{
	// static_cast<void>(ac);
	if (ac != 2)
	{
		Logger::error(std::cerr, "invalid argements.", "");
		return (1);
	}
	Parser parser(av[1]);
	ServerData serv(parser.getServers());
	ManageServers Manageservers(serv);
	Manageservers.runAllServers();
	Manageservers.acceptConection();
}




