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

//TODO 1. CAPIRE QUESTA COSA DEL SYMBOL
//TODO 2. TROVARE UN MODO MIGLIORE PER GESTIRE I CLIENT CONNESSI (ELIMINANDO LO STRUCT)
//TODO 3. OTTIMIZZARE IL CODICE E RENDERLO PIU' LEGGIBILE


Channel::Channel(std::string chName, std::string chKey, Client *chOperator) : \
    _chName(chName), _chKey(chKey), _chOperator(chOperator) {
    if (checkChName(chName))
        throw Channel::WrongCharacter();

    t_PChannel newClient;

    //* Questa parte deve essere un attimo rivista
    newClient.prefix = '~';
    newClient.modeLetter[0] = '+';
    newClient.modeLetter[1] = 'q';
    newClient.client = chOperator;

    this->_clientList.push_back(newClient);
    this->_symbol = '=';
    this->_clientLimit = 2147483647;
    this->_chMode = B;  //jfabi: non ricordo. Forse c'è qualche errore
    this->_clientNumber = 1;
    std::cout << "Channel created" << std::endl;
}

Channel::~Channel() {
    std::cout << "Channel deleted" << std::endl;
}

//* ################# GETTERS #################

std::string Channel::getName() const {
    return (this->_chName);
}

std::string Channel::getTopic() const {
    return (this->_topic);
}

Client *Channel::getClient(int fd) const {
    usr_pos it;

    for (it = this->_clientList.begin(); it != this->_clientList.end(); it++)
    {
        if ((*it).client->getSocketFd() == fd)
            return (*it).client;
    }
    return (NULL);
}

Client *Channel::getClient(std::string name) const {
    usr_pos it;

    for (it = this->_clientList.begin(); it != this->_clientList.end(); it++)
    {
        if (!(*it).client->getNickname().compare(name))
            return (*it).client;
    }
    return (NULL);
}

Client *Channel::getOperator() const {
    return (this->_chOperator);
}

Channel::usr_pos Channel::getFirstClient() const {
    return (this->_clientList.begin());
}

Channel::usr_pos Channel::getLastClient() const {
    return (this->_clientList.end());
}

t_PChannel  Channel::getT_PChannel(std::string name) const {
    usr_pos it;

    for (it = this->_clientList.begin(); it != this->_clientList.end(); it++)
    {
        if (!(*it).client->getNickname().compare(name))
            return (*it);
    }
    throw Channel::NoSuchChannel();
}

char	Channel::getSymbol() const {
    return (this->_symbol);
}

//* ################# OPERATIONS #################

int Channel::addClient(Client *client, std::string password, char prefix, char letter) {
    if (this->_chKey.compare(password))
        return (1);
    if (this->hasMode("+l") && this->_clientNumber >= this->_clientLimit)
        return (2);

    t_PChannel newClient;
    newClient.client = client;
    newClient.prefix = prefix;
    newClient.modeLetter[0] = '+';
    newClient.modeLetter[1] = letter;
    this->_clientList.push_back(newClient);
    this->_clientNumber += 1;
    return (0);
}

int Channel::removeClient(std::string CNick) {
    std::vector<t_PChannel>::iterator it;

    for (it = this->_clientList.begin(); it != this->_clientList.end(); it++)
    {
        if (!(*it).client->getNickname().compare(CNick))
        {
            this->_clientList.erase(it);
            this->_clientNumber -= 1;
            if (this->_clientNumber == 0)
                return (-1);
            return (0);
        }
    }
    return (1);
}

int Channel::removeClient(int fd) {
    std::vector<t_PChannel>::iterator it;

    for (it = this->_clientList.begin(); it != this->_clientList.end(); it++)
    {
        if ((*it).client->getSocketFd() == fd)
        {
            this->_clientList.erase(it);

            return (0);            
        }
    }
    return (1);
}

//! Ora come ora, non esiste una funzione che setti quel client come operator
void Channel::setOperator(const Client &client) {
}

void Channel::setMode(std::string m, int flag) {
    int bit;

    bit = this->ft_converter(m);
    if (bit <= 0)
        return ;
    if (flag  == 1)
        this->_chMode = this->_chMode ^ bit;
    else
        this->_chMode = this->_chMode || flag;
}

void Channel::addBanned(std::string nickname, std::string username) {
    this->_bannedClients.push_back(nickname + "!" + username);
}

void	Channel::setSymbol(char c) {
    this->_symbol = c;
}

void    Channel::setTopic(std::string newTopic) {
    this->_topic = newTopic;
}

int Channel::sendToAll(std::string text) {
    std::vector<t_PChannel>::iterator    it;
    std::vector<t_PChannel>::iterator    end;

    end = this->_clientList.end();
    for (it = this->_clientList.begin(); it < end; it++)
        send((*it).client->getSocketFd(), text.c_str(), text.size(), 0);
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

int Channel::hasMode(std::string m) {
    int bit;

    bit = this->ft_converter(m);
    return (this->_chMode & bit);
}

//* ################# PRIVATE #################

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
