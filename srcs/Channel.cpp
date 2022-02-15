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
#include "Client.hpp"

Channel::Channel(std::string chName, std::string chKey, Client *chOperator) : \
    _chName(chName), _chKey(chKey) {
    if (checkChName(chName))
        throw Channel::WrongCharacter();
    this->_symbol = '=';
    this->_clientLimit = 2147483647;
    this->_chMode = 0;  //jfabi: non ricordo. Forse c'è qualche errore
    this->_clientNumber = 1;
    this->_clients.push_back(std::pair<int, Client*>(O, chOperator));
    std::cout << "Channel created" << std::endl;
}

Channel::~Channel() {
    std::cout << "Channel deleted" << std::endl;
}

//* ################# GETTERS #################

std::string Channel::getName() const { return (this->_chName); }
std::string Channel::getTopic() const { return (this->_topic); }

Client *Channel::getClient(int fd) const {
    usr_pos it;

    for (it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        if ((*it).second->getSocketFd() == fd)
            return (*it).second;
    }
    return (NULL);
}

Client *Channel::getClient(std::string name) const {
    usr_pos it;

    for (it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        if (!(*it).second->getNickname().compare(name))
            return (*it).second;
    }
    return (NULL);
}

std::vector<Client *> Channel::getOperator() const 
{
    std::vector<Client *> ret;
    Channel::usr_pair_list::const_iterator  it;
    
    for (it = this->_clients.begin(); it < this->_clients.end(); it++)
    {
        if ((*it).first & O)
            ret.push_back((*it).second);
    }
    return (ret);
}

Channel::usr_pos Channel::getFirstClient() const { return (this->_clients.begin()); }
Channel::usr_pos Channel::getLastClient() const { return (this->_clients.end()); }

Channel::usr_pair  Channel::getPairClient(std::string name) const {
    usr_pos it;

    for (it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        if (!(*it).second->getNickname().compare(name))
            return (*it);
    }
    throw Channel::NoSuchChannel();
}

char	Channel::getSymbol() const { return (this->_symbol); }

//* ################# OPERATIONS #################

int Channel::addClient(Client *client, std::string password, char prefix, char letter) {
    if (this->_chKey.compare(password))
        return (1);
    if (this->hasMode('l') && this->_clientNumber >= this->_clientLimit)
        return (2);
    this->_clients.push_back(Channel::usr_pair(ft_client_converter(letter), client));
    this->_clientNumber += 1;
    return (0);
}

int Channel::removeClient(std::string CNick) {
    Channel::usr_pair_list::iterator it;

    for (it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        if (!(*it).second->getNickname().compare(CNick))
        {
            this->_clients.erase(it);
            this->_clientNumber -= 1;
            if (this->_clientNumber == 0)
                return (-1);
            return (0);
        }
    }
    return (1);
}

int Channel::removeClient(int fd) {
    Channel::usr_pair_list::iterator it;

    for (it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        if ((*it).second->getSocketFd() == fd)
        {
            this->_clients.erase(it);

            return (0);            
        }
    }
    return (1);
}

void Channel::setKey(std::string key, int flag)
{
    if (flag)
        this->_password = "";
    else
        this->_password = key;
}

void Channel::setOperator(std::string client, int flag)
{
    usr_pair_list::iterator it;

    for (it = this->_clients.begin(); it < this->_clients.end(); it++)
    {
        if ((*it).second->getNickname() == client)
        {
            if (flag)
                (*it).first = (*it).first ^ O;
            else
                (*it).first = (*it).first || O;
        }
    }
}

void Channel::setModeratorPermission(std::string client, int flag)
{
    usr_pair_list::iterator it;

    for (it = this->_clients.begin(); it < this->_clients.end(); it++)
    {
        if ((*it).second->getNickname() == client)
        {
            if (flag)
                (*it).first = (*it).first ^ V;
            else
                (*it).first = (*it).first || V;
        }
    }
}

int Channel::setMode(char m, int negative) {
    int bit;

    bit = this->ft_converter(m);
    if (bit <= 0)
        return (0);
    if (negative  == 1)
        this->_chMode = this->_chMode ^ bit;
    else
        this->_chMode = this->_chMode || bit;
    return (1);
}

void Channel::setBanMask(std::string mask, int flag)
{
    if (flag)
        this->_banMask = "";
    else
        this->_banMask = mask;
}

void Channel::addBanned(std::string nickname, std::string username) {
    this->_bannedClients.push_back(nickname + "!" + username);
}

void	Channel::setSymbol(char c) {
    this->_symbol = c;
}

void    Channel::setTopic(std::string newTopic) {
    if (newTopic == "\"\"")
        this->_topic = "";
    else
        this->_topic = newTopic;
}

int Channel::sendToAll(std::string text) {
    Channel::usr_pair_list::iterator    it;
    Channel::usr_pair_list::iterator    end;

    end = this->_clients.end();
    for (it = this->_clients.begin(); it < end; it++)
        send((*it).second->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}

void Channel::removeBanned(std::string nickname, std::string username) {
    std::vector<std::string>::iterator it;
    std::string bannedUser = nickname + "!" + username;

    for (it = this->_bannedClients.begin(); it < this->_bannedClients.end() ;it++)
    {
        if (!(*it).compare(bannedUser))
            this->_bannedClients.erase(it);
    }
}

//* ################# CHECKS #################

int Channel::isBanned(std::string nickname, std::string username) {
    std::vector<std::string>::iterator it;
    std::string bannedUser = nickname + "!" + username;

    for (it = this->_bannedClients.begin(); it < this->_bannedClients.end() ;it++)
    {
        if (!(*it).compare(bannedUser))
            return (1);
    }
    return (0);
}

int Channel::isOnChannel(std::string nickname)
{
    try
    {
        usr_pair client;
        client = this->getPairClient(nickname);
        return (1);
    }
    catch(const std::exception& e)
    {
        return (0);
    }   
}
int Channel::clientHasMode(std::string CNick, char c)
{
    usr_pair_list::iterator    it;

    try
    {
        usr_pair    client;
        client = this->getPairClient(CNick);
        if (client.first & ft_client_converter(c))
            return (1);
        return (0);
    }
    catch(const std::exception& e)
    {
        return (0);
    }
}

int Channel::hasMode(char m) {
    int bit;

    bit = this->ft_converter(m);
    return (this->_chMode & bit);
}

//* ################# PRIVATE #################

int	Channel::ft_converter(char c)
{
    if (c == 'b')
        return B;
    else if (c == 'e')
        return E;
    else if (c == 'l')
        return L;
    else if (c == 'i')
        return I;
    else if (c == 'I')
        return II;
    else if (c == 'k')
        return K;
    else if (c == 'm')
        return M;
    else if (c == 's')
        return S;
    else if (c == 't')
        return T;
    else if (c == 'n')
        return N;
    return 0;
}

int	Channel::ft_client_converter(char c)
{
    if (c == 'q')
        return Q;
    else if (c == 'a')
        return A;
    else if (c == 'o')
        return O;
    else if (c == 'h')
        return H;
    else if (c == 'v')
        return V;
    return 0;
}

int Channel::checkChName(std::string name)
{
    //? Controlliamo che il nome del canale sia corretto
    if (name == "" || (name[0] != '#' && name[0] != '&'))
        return 1;
    if (name.find_first_of(NOCHAR) != std::string::npos) //! QUESTO HA UN PROBLEMA CON IL BELL CHARACTER!
        return 2;
    /* for (int i = 0; i < name.size(); i++)
    {
        for (int j = 0; j < NOCHARSIZE; j++)
        {
            if (name[i] == NOCHAR[j])
                return (j);
        }
    } */
    return (0);
}

const char *Channel::NoSuchChannel::what() const throw() {
    return ("No Such Channel");
}

const char *Channel::WrongCharacter::what() const throw() {
    return ("Wrong Character");
}
