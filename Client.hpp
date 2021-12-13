/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 17:46:42 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/09 17:47:35 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client
{
    public:
        Client();
        Client(const Client &copy);
        ~Client();

        Client &operator=(const Client &copy);

        std::string getNickname() const;
        std::string getPassword() const;
        int         getSocketFd() const;
        bool        getAway() const;

        void        setNickname(std::string newname);
        void        setPassword(std::string newpassword);
        void        setSocketFd(int fd);
        void        setAway(bool flag);
    private:
        std::string nickname;
        std::string password;
        int         socket;
        bool        away;
        std::vector<std::string> capabilities;
};

std::ostream& operator<<(std::ostream& os, const Client &copy);

#endif
