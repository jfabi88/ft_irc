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

Channel::Channel(std::string chName, std::string key) : \
    _chName(chName), _chKey(key) {
    this->_clientLimit = 2147483647;
    this->_chMode = 0;  //jfabi: non ricordo. Forse c'è qualche errore
    this->_clientNumber = 0;
    //std::cout << "Channel created" << std::endl;
}

Channel::~Channel() {
    //std::cout << "Channel deleted" << std::endl;
}

//* ################# GETTERS #################

int Channel::getNClient() const { return (this->_clientNumber); }
int Channel::getLimit() const { return (this->_clientLimit); }
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

Client *Channel::getClient(std::string name) const
{
    usr_pos it;

    for (it = this->_clients.begin(); it < this->_clients.end(); it++)
    {
        if (!(*it).second->getNickname().compare(name))
            return (*it).second;
    }
    return (NULL);
}

std::vector<std::pair<int, Client *> >   Channel::getClients() { return (this->_clients); }

std::vector<Client *> Channel::getOperator()
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

Channel::usr_pair  Channel::getPairClient(std::string name) {
    usr_pos it;

    for (it = this->_clients.begin(); it < this->_clients.end(); it++)
    {
        if (!(*it).second->getNickname().compare(name))
            return (*it);
    }
    throw Channel::NoSuchChannel();
}

std::string Channel::getModes() const
{
    std::string mode;
    std::string param;
    std::vector<std::string>::const_iterator it;

    if (this->_chMode & B)
    {
        mode += 'b';
        for (it = this->_banMask.begin(); it < this->_banMask.end(); it++)
            param += " " + (*it);
    }
    if (this->_chMode & L)
    {
        mode += 'l';
        std::stringstream os;
        os << this->_clientLimit;
        param += " " + os.str();
    }
    if (this->_chMode & S)
        mode += 's';
    if (this->_chMode & I)
        mode += 'i';
    if (this->_chMode & T)
        mode += 't';
    if (this->_chMode & N)
        mode += 'n';
    if (this->_chMode & M)
        mode += 'm';
    if (this->_chMode & K)
        mode += 'k';
    return (mode + param);
}

std::string Channel::getAllMessages() const
{
    std::string text;

    text = "";
    for (std::vector<std::string>::const_iterator it = this->_messages.begin(); it < this->_messages.end(); it++)
        text.append((*it));
    return (text);
}

std::string Channel::getMaxPrefix(std::string client)
{
    std::pair<int, Client *>    tmp;

    tmp = this->getPairClient(client);
    if (tmp.first & O)
        return ("@");
    if (tmp.first & V)
        return ("+");
    return ("");
}

std::string Channel::getTopicSet() const { return (this->_topicSetter); }
std::string Channel::getTopicTime() const { return (this->_topicTime); }

//* ################# OPERATIONS #################

int Channel::addClient(Client *client, char letter)
{
    this->_clients.push_back(Channel::usr_pair(ft_client_converter(letter), client));
    this->_clientNumber += 1;
    return (0);
}

int Channel::addClient(Client *client, std::string password, char letter) {
    Channel::usr_pair toAdd;

    if (this->_chKey.compare(password))
        return (1);
    toAdd.first = ft_client_converter(letter);
    toAdd.second = client;
    this->_clients.push_back(toAdd);
    this->_clientNumber += 1;
    return (0);
}

int Channel::addInvitedClient(Client *client)
{
    if (!this->isInvited(client->getNickname()))
        this->_invitedClients.push_back(client);
    return (0);
}

int Channel::removeInvitedClient(Client *client)
{
    std::string cNick;

    cNick = client->getNickname();
    for (std::vector<Client *>::iterator it = _invitedClients.begin(); it < _invitedClients.end(); it++)
    {
        if (!(*it)->getNickname().compare(cNick))
        {
            _invitedClients.erase(it);
            return (0);
        }
    }
    return (1);
}

int Channel::removeClient(std::string CNick)
{
    Channel::usr_pair_list::iterator it;

    for (it = this->_clients.begin(); it < this->_clients.end(); it++)
    {
        if (!(*it).second->getNickname().compare(CNick))
        {
            this->_clients.erase(it);
            this->_clientNumber = this->_clientNumber - 1;
            return (this->_clientNumber);
        }
    }
    return (-1);
}

int Channel::removeClient(int fd) {
    Channel::usr_pair_list::iterator it;

    for (it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        if ((*it).second->getSocketFd() == fd)
        {
            this->_clients.erase(it);
            this->_clientNumber = this->_clientNumber - 1;
            return (this->_clientNumber);
        }
    }
    return (-1);
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
                (*it).first = (*it).first | O;
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
                (*it).first = (*it).first | V;
        }
    }
}

int Channel::setMode(char m, int negative) {
    int bit;

    bit = this->ft_converter(m);
    if (bit <= 0)
        return (0);
    if (negative  == 1)
    {
        this->_chMode = this->_chMode ^ bit;
        //if (m == 'i')
        //    this->emptyInvited();
    }
    else
        this->_chMode = this->_chMode | bit;
    return (1);
}

int Channel::setBanMask(std::string mask, int flag)
{
    size_t  n;

    if (flag)
        this->_banMask.clear();
    else
    {
        n = 0;
        n = mask.find('!', n);
        if (n == std::string::npos)
            return (1);
        n = mask.find('@',n);
        if (n == std::string::npos)
            return (1);
        if (mask[n + 1] == 0)
            return (1);
        this->_banMask.push_back(mask);
    }
    return (0);
}

void    Channel::setTopic(std::string newTopic) {
    if (newTopic == "")
        this->_topic = "";
    else
        this->_topic = newTopic;
}

void    Channel::emptyInvited() { _invitedClients.clear(); }

int Channel::sendToAll(std::string text) {
    std::vector<std::pair<int, Client *> >::const_iterator it;

    for (it = this->getFirstClient(); it < this->getLastClient(); it++)
        send((*it).second->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}

int Channel::sendToAll(std::string text, Client *client) {
    std::vector<std::pair<int, Client *> >::const_iterator it;

    for (it = this->getFirstClient(); it < this->getLastClient(); it++)
    {
        std::cout << "Il testo inviato é: " << text << std::endl;
        if ((*it).second != client)
            send((*it).second->getSocketFd(), text.c_str(), text.size(), 0);
    }
    return (0);
}

void Channel::setLimit(int limit) { this->_clientLimit = limit; };

std::string Channel::ltop(int c)
{
    if (c & V)
        return ("+");
    else if (c & O)
        return ("@");
    return ("");
};

void Channel::setTopicSetter(std::string setter) { this->_topicSetter = setter; }
void Channel::setTopicTime()
{
    time_t result = time(NULL);
    std::stringstream os;
    os << result;
    this->_topicTime = os.str();
}

void Channel::addMessage(std::string message) { this->_messages.push_back(message); }

//* ################# CHECKS #################

int Channel::isBanned(std::string nickname, std::string username, std::string realname)
{
    std::vector<std::string>::iterator it;
    std::string bannedUser = nickname + "!" + username + '@' + realname;

    for (it = this->_banMask.begin(); it < this->_banMask.end() ;it++)
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

int Channel::checkBanMask(std::string banMask)
{
    int posE;
    int posC;

    if (banMask == "")
        return (0);
    if (banMask.find('!') == std::string::npos)
        return (1);
    posE = banMask.find('!');
    if (banMask.find('!', posE) == std::string::npos)
        return (1);
    if (banMask.find('@') == std::string::npos)
        return (1);
    posC = banMask.find('@');
    if (banMask.find('@', posE) == std::string::npos)
        return (1);
    if (posC > posE)
        return (1);
    return (0);
}

int Channel::hasMode(char m) {
    int bit;

    bit = this->ft_converter(m);
    return (this->_chMode & bit);
}

int Channel::isInvited(std::string cNick)
{
    for (std::vector<Client *>::iterator it = _invitedClients.begin(); it < _invitedClients.end(); it++)
    {
        if (!(*it)->getNickname().compare(cNick))
            return (1);
    }
    return (0);
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
    if (c == 'o')
        return O;
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
