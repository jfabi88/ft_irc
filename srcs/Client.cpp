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

//* ::smenna
//? Ho usato le initialization list per ridurre le linee di codice e rendere tutto più pulito

Client::Client() : \
    _nickname(""), _password(""), _username(""), _realname(""), _subChannelsNum(0),
    _socket(-1), _access(0), _recFlag(0), _isAway(false), _isRegistered(false) {
    std::cout << "Client constructor called" << std::endl;
}

Client::Client(const Client &copy) : \
    _nickname(copy.getNickname()), _password(copy.getPassword()), _subChannelsNum(copy.getChannelSub()), 
    _socket(copy.getSocketFd()), _access(copy.getAccess()), _isAway(copy.getAwayStatus()) {
    std::cout << "Client copy constructor called" << std::endl;
}

Client::~Client() {
    std::cout << "Client destructor called" << std::endl;
}

Client &Client::operator=(const Client &copy) {
    if (this == &copy)
        return (*this);

    this->_nickname = copy.getNickname();
    this->_password = copy.getPassword();
    this->_socket = copy.getSocketFd();
    this->_access = copy.getAccess();
    this->_subChannelsNum = copy.getChannelSub();

    std::cout << "Client overload (operator=) called" << std::endl;
    return (*this);
}

bool Client::operator==(const Client &secondClient) const
{
    if (this->_nickname.compare(secondClient.getNickname()) == 0)
        return (true);
    return (false);
}

bool Client::operator!=(const Client &secondClient) const
{
    if (this->_nickname.compare(secondClient.getNickname()) == 0)
        return (false);
    return (true);
}

//* ################# GETTERS #################

std::string Client::getNickname() const { return (this->_nickname); }
std::string Client::getPassword() const { return (this->_password); }
std::string Client::getUsername() const { return (this->_username); }
std::string Client::getRealname() const { return (this->_realname); }

Channel *Client::getChannel(int indx) const {

    ch_iter it;
    int i = 0;

    for (it = this->_subChannels.begin(); it != this->_subChannels.end() ;it++)
    {
        if (i  == indx)
            return (*it);
        i++;
    }
    return (NULL);
}

Channel *Client::getChannel(std::string name) const {

    ch_iter it;

    for (it = this->_subChannels.begin(); it != this->_subChannels.end() ;it++)
    {
        if (!(*it)->getName().compare(name))
            return (*it);
    }

    return (NULL);
}

Client::ch_iter Client::getFirstChannel() const             { return (this->_subChannels.begin()); }
Client::ch_iter Client::getLastChannel() const              { return (this->_subChannels.end());   }

std::vector<std::string> Client::getCapabilities() const    { return (this->_capabilities); }
std::string Client::getAwayMessage() const                  { return (this->_awayMessage); }

bool Client::getAwayStatus() const                          { return (this->_isAway); }
bool Client::getRegisteredStatus() const                    { return (this->_isRegistered); }
int Client::getSocketFd() const                             { return (this->_socket); }
int Client::getChannelSub() const                           { return (this->_subChannelsNum); }
int Client::getAccess() const                               { return (this->_access); }
int Client::getRecFlag() const                              {return (this->_recFlag);}

//* ################# SETTERS #################

void Client::setNickname(std::string newName)     { this->_nickname = newName; }
void Client::setPassword(std::string newPassword) { this->_password = newPassword; }
void Client::setUsername(std::string newUsername) { this->_username = newUsername; }
void Client::setRealname(std::string newRealname) { this->_realname = newRealname; }
void Client::addChannel(Channel *newChannel)      { this->_subChannels.push_back(newChannel); }

void Client::removeChannel(std::string channelName)
{
    std::vector<Channel *>::iterator it;

    for (it = this->_subChannels.begin(); it != this->_subChannels.end() ;it++)
    {
        if (!(*it)->getName().compare(channelName))
        {
            this->_subChannels.erase(it);
            return ;
        }
    }   
}

void Client::setMode(char c, int flag)
{
    if (flag)
        this->_mode = this->_mode ^ ft_converter(c);
    else
        this->_mode = this->_mode | ft_converter(c);
}

void Client::setAway(bool status, std::string message) {
    this->_isAway = status;
    this->_awayMessage = message;
}

void Client::setCapabilities(str_list newVector) {
    this->_capabilities.clear();
    this->_capabilities = newVector;
}

void Client::setRegistered(bool flag) { this->_isRegistered = flag; }
void Client::setSocketFd(int fd)      { this->_socket = fd; }
void Client::setAccess(int flag)      { this->_access = flag; }
void Client::setRecFlag(int flag)     {this->_recFlag = flag;}
//* ################# CHECKS #################

int Client::hasCapability(std::string name) const
{
    std::vector<std::string>::const_iterator  it;
    std::string tmp;
    int         i;

    i = 0;
    it = this->_capabilities.begin();
    tmp = name;
    if (name != "" && name[0] == '-')
        tmp = name.substr(1, name.size());
    while (!(*it).compare(tmp) && it != this->_capabilities.end())
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

int Client::hasMode(char c) const
{
    int n;

    n = this->ft_converter(c);
    return (this->_mode & n);
}

int Client::isOperator(Channel *channel) const
{
    std::vector<Client *> op;
    std::vector<Client *>::const_iterator it;

    op = channel->getOperator();
    for (it = op.begin(); it < op.end(); it++)
    {
        if ((*it)->getNickname().compare(this->getNickname()) == 0)
            return (1);
    }
    return (0);
}

int Client::ft_converter(char c) const
{
    if (c == 'i')
        return UI;
    else if (c == 's')
        return US;
    else if (c == 'w')
        return UW;
    else
        return 0;
}

//* ################# EXTERNAL FUNCTIONS #################

std::ostream& operator<<(std::ostream& os, const Client &copy)
{
    os << "Nickname: " << copy.getNickname() << "\n";
    os << "User: " << copy.getUsername() << "\n";
    os << "RealName: " << copy.getRealname() << "\n";
    os << "Socket: " << copy.getSocketFd() << "\n";
    return (os);
}
