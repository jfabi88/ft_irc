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
#include <vector>

class Client
{
    public:
        Client();
        Client(const Client &copy);
        ~Client();

        Client &operator=(const Client &copy);

        std::string getNickname() const;
        std::string getPassword() const;
        std::string getUsername() const;
        std::string getRealname() const;
        int         getSocketFd() const;
        bool        getAway() const;
        bool        getRegistered() const;
        std::string getAwayMessage() const;
        std::vector<std::string> getCapabilities() const;

        void        setNickname(std::string newname);
        void        setPassword(std::string newpassword);
        void        setUsername(std::string newusername);
        void        setRealname(std::string newrealname);
        void        setSocketFd(int fd);
        void        setAway(bool flag, std::string message);
        void        setCapabilities(std::vector<std::string> newVector);
        void        setRegistered(bool flag);
        int hasCapability(std::string name) const;
        int hasCapabilities(std::vector<std::string> prefix) const;
    private:
        std::string nickname;
        std::string password;
        std::string username;
        std::string realname;
        std::string awayMessage;
        int         socket;
        bool        away;
        bool        registered;
        std::vector<std::string> capabilities;
};

std::ostream& operator<<(std::ostream& os, const Client &copy);

#endif
