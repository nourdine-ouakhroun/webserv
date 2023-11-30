/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDepandenc.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nouakhro <nouakhro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 19:51:28 by nouakhro          #+#    #+#             */
/*   Updated: 2023/11/30 23:39:39 by nouakhro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEDEPANDENC_HPP
#define FILEDEPANDENC_HPP
#include<iostream>
#include<poll.h>
class FileDepandenc
{
private:
    std::string requist;
    int status;
public:
    const std::string &getRequist() const;
    const int &getStatus() const;
    void setRequist(const std::string);
    void setStatus(const int &);
    std::string boundery;
    pollfd fdpoll;
};

#endif