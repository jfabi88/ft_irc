/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 17:47:44 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/09 17:47:58 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/**CONSTRUCTOR**/

Client::Client()
{
    this->nickname = "";
    this->password = "";
    this->socket = -1;
    this->away = false;
    std::cout << "Client created" << std::endl;
}

Client::Client(const Client &copy)
{
    this->nickname = copy.getNickname();
    this->password = copy.getPassword();
    this->socket = copy.getSocketFd();
    this->away = copy.getAway();
    std::cout << "Client created" << std::endl;
}

Client::~Client()
{
    std::cout << "Client delated" << std::endl;
}

Client &Client::operator=(const Client &copy)
{
    if (this == &copy)
        return (*this);
    this->nickname = copy.getNickname();
    this->password = copy.getPassword();
    this->socket = copy.getSocketFd();    
    std::cout << "Client created" << std::endl;
    return (*this);
}

/**GET-SET**/

std::string Client::getNickname() const
{
    return (this->nickname);
}

std::string Client::getPassword() const
{
    return (this->password);
}

int Client::getSocketFd() const
{
    return (this->socket);
}

bool Client::getAway() const
{
    return (this->away);
}
std::vector<std::string> Client::getCapabilities() const
{
    return (this->capabilities);
}

void Client::setNickname(std::string newname)
{
    this->nickname = newname;
}

void Client::setPassword(std::string newpassword)
{
    this->password = newpassword;
}

void Client::setSocketFd(int fd)
{
    this->socket = fd;
}

void Client::setAway(bool flag)
{
    this->away = flag;
}

int Client::hasCapability(std::string name) const
{
    std::vector<std::string>::const_iterator  it;
    std::string tmp;

    it = this->capabilities.begin();
    tmp = name;
    if (name != "" && name[0] == '-')
        tmp = name.substr(1, name.size());
    while (!(*it).compare(tmp) && it != this->capabilities.end())
        it++;
    if ((*it) != "")
        return (1);
    return (0);
}

int     Client::hasCapabilities(std::vector<std::string> prefix) const
{
    std::vector<std::string>::iterator it;

    for (it = prefix.begin(); it != prefix.end(); it++)
    {
        if (!this->hasCapability(*it))
            return (0);
    }
    return (1);
}

/**EXTERNAL-FUNCTIONS**/

std::ostream& operator<<(std::ostream& os, const Client &copy)
{
    os << "Nickname: " << copy.getNickname() << "\n";
    os << "Password: " << copy.getPassword() << "\n";
    os << "Socket: " << copy.getSocketFd() << "\n";
    return (os);
}
