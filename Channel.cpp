/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/30 10:07:44 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/30 10:08:04 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, std::string key, Client *clientOperator)
{
    if (ft_test_name(name))
        throw Channel::WrongCharacter();
    this->name = name;
    this->chop = clientOperator;
    this->password = key;
    this->listClient.push_back(clientOperator);
    mode = 0;
    std::cout << "Channel created" << std::endl;
}

Channel::~Channel()
{
    std::cout << "Channel deleted" << std::endl;
}

std::string Channel::getName() const
{
    return (this->name);
}

Client *Channel::getClient(int fd) const
{
    std::vector<Client *>::const_iterator it;

    for (it = this->listClient.begin(); it != this->listClient.end(); it++)
    {
        if ((*it)->getSocketFd() == fd)
            return (*it);
    }
    return (NULL);
}

Client *Channel::getClient(std::string name) const
{
    std::vector<Client *>::const_iterator it;

    for (it = this->listClient.begin(); it != this->listClient.end(); it++)
    {
        if (!(*it)->getNickname().compare(name))
            return (*it);
    }
    return (NULL);
}

Client *Channel::getOperator() const
{
    return (this->chop);
}
		
int Channel::addClient(Client *client, std::string password)
{
    if (this->password.compare(password))
        return (1);
    this->listClient.push_back(client);
    return (0);
}

void Channel::setMode(std::string m, int flag)
{
    int bit;

    bit = this->ft_converter(m);
    if (bit <= 0)
        return ;
    if (flag  == 1)
        this->mode = this->mode ^ bit;
    else
        this->mode = this->mode || flag;
}

void Channel::addBanned(std::string CNick, std::string cUser)
{
    this->banned.push_back(CNick + "!" + cUser);
}

void Channel::removeBanned(std::string CNick, std::string cUser)
{
    std::vector<std::string>::iterator it;
    std::string banUser = CNick + "!" + cUser;

    for (it = this->banned.begin(); it < this->banned.end() ;it++)
    {
        if (!(*it).compare(banUser))
            this->banned.erase(it);
    }
}

int Channel::isBanned(std::string CNick, std::string CUser)
{
    std::vector<std::string>::iterator it;
    std::string banUser = CNick + "!" + CUser;

    for (it = this->banned.begin(); it < this->banned.end() ;it++)
    {
        if (!(*it).compare(banUser))
            return (1);
    }
    return (0);
}

int Channel::hasMode(std::string m)
{
    int bit;

    bit = this->ft_converter(m);
    return (this->mode && bit);
}

/****PRIVATE*****/

int	Channel::ft_converter(std::string m)
{
    if (!m.compare("+b"))
        return B;
    else if (!m.compare("+e"))
        return E;
    else if (!m.compare("+l"))
        return L;
    else if (!m.compare("+i"))
        return I;
    else if (!m.compare("+I"))
        return II;
    else if (!m.compare("+k"))
        return K;
    else if (!m.compare("+m"))
        return M;
    else if (!m.compare("+s"))
        return S;
    else if (!m.compare("t"))
        return T;
    else if (!m.compare("n"))
        return N;
    return 0;
}

int Channel::ft_test_name(std::string name)
{
    if (name == "" || (name[0] != '#' && name[0] != '&'))
        return (-1);
    for (int i = 0; i < name.size(); i++)
    {
        for (int j = 0; j < NOCHARSIZE; j++)
        {
            std::cout << "Il valore " << j << " è: " << NOCHAR[j] << std::endl;
            if (name[i] == NOCHAR[j])
                return (j);
        }
    }
    return (0);
}

const char *Channel::WrongCharacter::what() const throw()
{
    return ("wrong character");
}
