/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nouakhro <nouakhro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 16:41:57 by nouakhro          #+#    #+#             */
/*   Updated: 2023/11/28 16:48:52 by nouakhro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include"ManageServers.hpp"

int main(int ac, char **av)
{
	static_cast<void>(ac);
	Parser parser(av[1]);
	ServerData serv(parser.getServers());
	ManageServers Manageservers(serv);
	Manageservers.runAllServers();
	Manageservers.acceptConection();
}




