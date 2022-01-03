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
#include "Channel.hpp"

/**CONSTRUCTOR**/

Client::Client()
{
    this->nickname = "";
    this->password = "";
    this->username = "";
    this->realname = "";
    this->socket = -1;
    this->away = false;
    this->registered = false;
    this->access = 0;
    this->channelSub = 0;
    std::cout << "Client created" << std::endl;
}

Client::Client(const Client &copy)
{
    this->nickname = copy.getNickname();
    this->password = copy.getPassword();
    this->socket = copy.getSocketFd();
    this->away = copy.getAway();
    this->access = copy.getAccess();
    this->channelSub = copy.channelSub;
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
    this->access = copy.getAccess();
    this->channelSub = copy.getChannelSub();
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

std::string Client::getUsername() const
{
    return (this->username);
}

std::string Client::getRealname() const
{
    return (this->realname);
}

Channel *Client::getChannel(int indx) const
{
    int i;
    std::vector<Channel *>::const_iterator it;

    i = 0;
    for (it = this->channels.begin(); it != this->channels.end() ;it++)
    {
        if (i  == indx)
            return (*it);
        i++;
    }
    return (NULL);
}

Channel *Client::getChannel(std::string name) const
{
    std::vector<Channel *>::const_iterator it;

    for (it = this->channels.begin(); it != this->channels.end() ;it++)
    {
        if (!(*it)->getName().compare(name))
            return (*it);
    }
    return (NULL);
}

int Client::getSocketFd() const
{
    return (this->socket);
}

int Client::getAccess() const
{
    return (this->access);
}

int Client::getChannelSub() const
{
    return (this->channelSub);
}

bool Client::getAway() const
{
    return (this->away);
}

bool Client::getRegistered() const
{
    return (this->registered);
}

std::string Client::getAwayMessage() const
{
    return (this->awayMessage);
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

void Client::setAccess(int flag)
{
    this->access = flag;
}

void Client::setAway(bool flag, std::string message)
{
    this->away = flag;
    this->awayMessage = message;
}

void Client::setUsername(std::string newusername)
{
    this->username = newusername;
}

void Client::setRealname(std::string newrealname)
{
    this->realname = newrealname;
}

void Client::setCapabilities(std::vector<std::string> newVector)
{
    this->capabilities.clear();
    this->capabilities = newVector;
}

void Client::setRegistered(bool flag)
{
    this->registered = flag;
}

void Client::addChannel(Channel *newChannel)
{
    this->channels.push_back(newChannel);
}

void Client::removeChannel(std::string channelName)
{
    std::vector<Channel *>::const_iterator it;

    for (it = this->channels.begin(); it != this->channels.end() ;it++)
    {
        if (!(*it)->getName().compare(name))
        {
            this->channels.erase(it);
            return ;
        }
    }   
}

int Client::hasCapability(std::string name) const
{
    std::vector<std::string>::const_iterator  it;
    std::string tmp;
    int         i;

    i = 0;
    it = this->capabilities.begin();
    tmp = name;
    if (name != "" && name[0] == '-')
        tmp = name.substr(1, name.size());
    while (!(*it).compare(tmp) && it != this->capabilities.end())
    {
        it++;
        i++;
    }
    if ((*it) != "")
        return (i);
    return (-1);
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
