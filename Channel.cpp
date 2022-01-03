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
    t_PChannel newClient;
    this->name = name;
    this->chop = clientOperator;
    this->password = key;
    this->topic = "";
    newClient.prefix = '~';
    newClient.modeLetter[1] = 'q';
    this->listClient.push_back(newClient);
    this->symbol = '=';
    this->limit = 2147483647;
    this->mode = 1;  //jfabi: non ricordo. Forse c'è qualche errore
    this->numberClient = 1;
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
    std::vector<t_PChannel>::const_iterator it;

    for (it = this->listClient.begin(); it != this->listClient.end(); it++)
    {
        if ((*it).client->getSocketFd() == fd)
            return (*it).client;
    }
    return (NULL);
}

Client *Channel::getClient(std::string name) const
{
    std::vector<t_PChannel>::const_iterator it;

    for (it = this->listClient.begin(); it != this->listClient.end(); it++)
    {
        if (!(*it).client->getNickname().compare(name))
            return (*it).client;
    }
    return (NULL);
}

char	Channel::getSymbol() const
{
    return (this->symbol);
}

Client *Channel::getOperator() const
{
    return (this->chop);
}

std::vector<t_PChannel>::const_iterator Channel::getFirstClient() const
{
    return (this->listClient.begin());
}

std::vector<t_PChannel>::const_iterator Channel::getLastClient() const
{
    return (this->listClient.end());
}

std::string Channel::getTopic() const
{
    return (this->topic);
}

int Channel::addClient(Client *client, std::string password, char prefix, char letter)
{
    if (this->password.compare(password))
        return (1);
    if (this->hasMode("+l") && this->numberClient >= this->limit)
        return (2);
    t_PChannel newClient;
    newClient.client = client;
    newClient.prefix = prefix;
    newClient.modeLetter[1] = letter;
    this->listClient.push_back(newClient);
    this->numberClient += 1;
    return (0);
}

int Channel::removeClient(std::string CNick)
{
    std::vector<t_PChannel>::const_iterator it;

    for (it = this->listClient.begin(); it != this->listClient.end(); it++)
    {
        if (!(*it).client->getNickname().compare(CNick))
        {
            this->listClient.erase(it);
            this->numberClient -= 1;
            if (this->numberClient == 0)
                return (-1);
            return (0);
        }
    }
    return (1);
}

int Channel::removeClient(int fd)
{
    std::vector<t_PChannel>::const_iterator it;

    for (it = this->listClient.begin(); it != this->listClient.end(); it++)
    {
        if ((*it).client->getSocketFd() == fd)
        {
            this->listClient.erase(it);

            return (0);            
        }
    }
    return (1);
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

void	Channel::setSymbol(char c)
{
    this->symbol = c;
}

void    Channel::setTopic(std::string newTopic)
{
    this->topic = newTopic;
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
