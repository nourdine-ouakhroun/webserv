/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDepandenc.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nouakhro <nouakhro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 19:51:28 by nouakhro          #+#    #+#             */
/*   Updated: 2023/12/06 12:16:20 by nouakhro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEDEPANDENC_HPP
#define FILEDEPANDENC_HPP
#include<iostream>
#include<poll.h>
class FileDepandenc
{
private:
public:
    FileDepandenc();
    ~FileDepandenc();
    // const std::string &getRequist() const;
    // void setRequist(const std::string);
    // void eraseRequist(size_t _n, size_t linght);
    std::string requist;
    std::string rest;
    std::string filtred;
    std::string boundery;
    bool status;
    int fd;
    // std::string header;
    size_t lenght;
    size_t contenlenght;
    pollfd fdpoll;
};

#endif