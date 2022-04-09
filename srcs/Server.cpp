#include "Server.hpp"

/**CONSTRUCTOR**/

Server::Server()
{
    this->_servername = "IRC1ONE";
    this->_date = this->returnDate();
    this->_password = "";
    this->_motd = "<3 <3 <3";

    this->_awaylen = 256;
    this->_casemapping = "ascii";
    this->_chanlimit = "#:&:";
    this->_chanlen = 32;
    this->_chantypes = "#&";
    this->_elist = "U";
    //EXCEPTS
    //EXTBAN
    this->_hostlen = 64;
    //INVEX
    this->_kicklen = 256;
    //MAXLIST
    this->_maxtargets = -1;
    this->_modes = -1;
    this->_network = "network";
    this->_nicklen = 30;
    //PREFIX
    this->_safelist = true;
    //STATUSMSG
    //TARGMAX
    this->_topiclen = 307;
    this->_userlen = 16;

    std::cout << "Server created" << std::endl;
}

Server::Server(int port, int fd, std::string password)
{
    this->_port = port;
    this->_fd = fd;
    this->_password = password;
    this->_date = this->returnDate();
    this->_servername = "IRC1ONE";
}

/**GET-SET**/

int     Server::getPort() const
{
    return (this->_port);
}

int     Server::getSocket() const
{
    return (this->_fd);
}

std::string Server::getServername() const
{
    return (this->_servername);
}

std::string Server::getVersion() const
{
    return (this->_version);
}

std::string Server::getMotD() const
{
    return (this->_motd);
}

std::vector<Client *> Server::getClients()
{
    return (this->_clients);
}

std::vector<Client *>::const_iterator   Server::getItClients(std::string cNick) const
{
    std::vector<Client *>::const_iterator   it;

    it = this->_clients.begin();
    while ((*it)->getNickname().compare(cNick))
        it++;
    return (it);
}

std::vector<Client *>::iterator         Server::getItNcClients(std::string cNick)
{
    std::vector<Client *>::iterator   it;

    it = this->_clients.begin();
    while ((*it)->getNickname().compare(cNick))
        it++;
    return (it);   
}

Client  *Server::getClient(int fd) const
{
    std::vector<Client *>::const_iterator it;

    for (it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        if ((*it)->getSocketFd() == fd)
        {
            return (*it);
        }
    }
    return (NULL);
}
std::string Server::getDate() const
{
    return (this->_date);
}

Client  *Server::getClient(std::string name)
{
    std::vector<Client *>::const_iterator it;

    for (it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        if ((*it)->getNickname() == name)
        {
            return (*it);
        }
    }
    return (NULL);
}

std::vector<std::string> Server::getCapabilities()
{
    return (this->_capabilities);
}

std::vector<std::string> Server::getParameter()
{
    std::vector<std::string> ret;
    std::string text;
    std::string textInt;

    text = "AWAYLEN=";
    textInt = _awaylen;
    text.append(textInt);
    ret.push_back(text);
    text = "USERLEN=";
    textInt = _userlen;
    text.append(textInt);
    ret.push_back(text);
    ret.push_back("CASEMAPPING=" + _casemapping);
    ret.push_back("CHANLIMIY=" + _chanlimit);
    text = "CHANLEN=";
    textInt = _chanlen;
    text.append(textInt);
    ret.push_back(text);
    ret.push_back("CHANTYPES=" + _chantypes);
    ret.push_back("ELIST=" + _elist);
    text = "HOSTLEN=";
    textInt = _hostlen;
    text.append(textInt);
    ret.push_back(text);
    text = "KICKLEN";
    textInt = _kicklen;
    text.append(textInt);
    ret.push_back(text);
    ret.push_back("MAXTARGETS=");
    ret.push_back("MODES=");
    ret.push_back("ANETWORK=" + _network);
    text = "NICKLEN";
    textInt = _nicklen;
    text.append(textInt);
    ret.push_back(text);
    ret.push_back("SAFELIST");
    text = "TOPICLEN";
    textInt = _topiclen;
    text.append(textInt);
    ret.push_back(text);
    text = "USERLEN";
    textInt = _topiclen;
    text.append(textInt);
    ret.push_back(text);
    return (ret);
}

Channel *Server::getChannel(std::string name)
{
    std::vector<Channel *>::iterator it;

    for (it = this->_channels.begin(); it != this->_channels.end() ;it++)
    {
        if (!(*it)->getName().compare(name))
            return (*it);
    }
    return (NULL);
}

std::vector<Channel *>                  Server::getChannels() { return (this->_channels); }
std::vector<Channel *>::const_iterator  Server::getFirstChannel() const { return (this->_channels.begin()); }
std::vector<Channel *>::const_iterator  Server::getLastChannel() const { return (this->_channels.end()); }

std::string Server::returnDate() const
{
    time_t          now;
    std::string     ret;

    now = time(0);
    tm *gt = gmtime(&now);
    char *dt = asctime(gt);
    ret.append(dt);
    ret = ret.substr(0, ret.size() - 1);
    ret.append(" UTC");
    return (ret);
}

void    Server::setPort(int newport)
{
    this->_port = newport;
}

void    Server::setSocket(int newfd)
{
    this->_fd = newfd;
}

void    Server::setServername(std::string newservername)
{
    this->_servername = newservername;
}

void    Server::setVersion(std::string newversion)
{
    this->_version = newversion;
}

void    Server::setClient(Client *newclient)
{
    this->_clients.push_back(newclient);
}

void    Server::setMotD(std::string motd)
{
    this->_motd = motd;
}

int    Server::verifyPassword(std::string userPassword)
{
    if (this->_password == "")
        return (1);
    else if (!userPassword.compare(this->_password))
        return (1);
    return (0);
}

void    Server::addChannel(Channel *channel)
{
    this->_channels.push_back(channel);
}

void    Server::removeChannel(std::string channelName)
{
    std::vector<Channel *>::iterator it;

    for (it = this->_channels.begin(); it < this->_channels.end(); it++)
    {
        if (!((*it)->getName().compare(channelName)))
        {
            this->_channels.erase(it);
            this->_chanlen -= 1;
            //delete (*it);
            return ;
        }
    }
}

void    Server::removeClient(std::string clientName)
{
    std::vector<Client *>::iterator it;

    for (it = this->_clients.begin(); it < this->_clients.end(); it++)
    {
        if (!((*it)->getNickname().compare(clientName)))
        {
            this->_clients.erase(it);
            //delete (*it);
            return ;
        }
    }    
}

void    Server::quitClient(int fd)
{
    Message     message;
    std::string text;
    std::vector<Channel *>  channels;
    Client      *client;

    client = this->getClient(fd);
    if (client == NULL)
        return ;
    text = ":" + client->getNickname() + "QUIT :connection closed";
    text += DEL;
    channels = client->getChannels();
    for (std::vector<Channel *>::const_iterator it = channels.begin(); it < channels.end(); it++)
    {
        if ((*it)->removeClient(client->getSocketFd()) == 0)
            this->removeChannel((*it)->getName());
        else
            (*it)->sendToAll(text, client);
    }
    this->removeClient(client->getNickname());
    return ;
}

/**PUBLIC-FUNCTIONS**/

int Server::hasCapability(std::string name) const
{
    std::vector<std::string>::const_iterator  it;
    std::string tmp;

    it = this->_capabilities.begin();
    if (it == this->_capabilities.end())
        return (0);
    tmp = name;
    if (name != "" && name[0] == '-')
        tmp = name.substr(1, name.size());
    while (!(*it).compare(tmp) && it != this->_capabilities.end())
        it++;
    if ((*it) != "")
        return (1);
    return (0);
}

int     Server::hasCapabilities(std::vector<std::string> prefix) const
{
    std::vector<std::string>::iterator it;

    for (it = prefix.begin(); it != prefix.end(); it++)
    {
        if (!this->hasCapability(*it))
            return (0);
    }
    return (1);
}

int     Server::haveChannelCommon(Client *firstClient, Client *secondClient)
{
    std::vector<Channel *>::const_iterator  ft;
    std::vector<Channel *>::const_iterator  st;
    std::vector<Channel *>  listone;
    std::vector<Channel *>  listtwo;

    listone = firstClient->getChannels();
    listtwo = secondClient->getChannels();
    for (ft = listone.begin(); ft < listone.end(); ft++)
    {
        for (st = listtwo.begin(); st < listtwo.end(); st++)
        {
            if ((*ft)->getName().compare((*st)->getName()) == 0)
                return (1);
        }
    }
    return (0);
}

int     Server::findClient(std::string nickname) const
{
    std::vector<Client *>::const_iterator it;
    int i;

    i = 0;
    it = this->_clients.begin();
    for (it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        if ((*it)->getNickname().compare(nickname) == 0)
            return (i);
        i++;
    }
    return (-1);
}

int Server::startCommunication(int fdNewClient, char *buffer, Client *client)
{
    int flag;
    std::vector<std::string> array;

    flag = client->getRecFlag();
    if (flag != 3 && flag != 7)
    {
        array = ft_take_messages(fdNewClient, buffer);
        for (std::vector<std::string>::iterator it = array.begin();it != array.end(); it++)
        {
            flag = this->ft_exec_communication_commands(flag, *it, client);
            if (flag == -1)
                return (0);
            client->setRecFlag(flag);
        }
    }
    if (flag == 3 || flag == 7)
    {
        if (this->_password != "" && !client->getAccess())
        {
            std::string text = makePasswdMisMatch(client->getNickname());
            send(fdNewClient, text.c_str(), text.size(), 0);
            this->_clients.erase(this->getItNcClients(client->getNickname()));
            //delete client;
            return (0);
        }
        client->setRegistered(true);
        ft_welcome(client);
    }
    return (0);
}

int     Server::receiveCommand(int fdClient, char *buffer)
{
    std::vector<std::string> array;
    Message message;
    Client  *client;

    std::cout << "Il messaggio ricevuto é: " << buffer << std::endl;
    client = this->getClient(fdClient);
    array = ft_take_messages(fdClient, buffer);
    for (std::vector<std::string>::iterator it = array.begin(); it != array.end(); it ++)
    {
        message.setMessage(*it);
        execCommand(message, client, this);
    }
    return (0);
}

void    Server::printClients()
{
    std::vector<Client *>::iterator it;

    for (it = this->getClients().begin(); it != this->getClients().end(); it++)
        std::cout << "Nick: " << (*it)->getNickname() << std::endl;
}

int     Server::badChanMask(std::string channelname)
{
    std::string mask;

    mask = CHANMASK;
    for (size_t i = 0; i < mask.size(); i++)
    {
        if (channelname.find(mask[i], 1) != std::string::npos)
            return (1);
    }
    return (0);
}

/**PRIVATE-FUNCTIONS**/

void Server::ft_memset(char *buffer, int size)
{
    for (int i = 0; i < size ;i++)
        buffer[i] = 0;
}

std::vector<std::string> Server::ft_take_messages(int fdClient, char *buffer)
{
    std::vector<std::string> array;
    std::string b;

    b = "";
    if (buffer[0] == 0)
        recv(fdClient, buffer, 512, 0);   //jfabi: a che serve questa cosa?
    this->ft_parse_data(&array, &b, buffer);
    ft_memset(buffer, 512);
    while (b != "")                        //jfabi: questo serve per il multilines, cosa che noi non dobbiamo gestire per forza
    {
        recv(fdClient, buffer, 512, 0);
        this->ft_parse_data(&array, &b, buffer);
        ft_memset(buffer, 512);  
    }
    return (array);
}

void Server::ft_parse_data(std::vector<std::string> *array, std::string *b, char *buffer)
{
    int num;
    size_t lastIndx;
    std::string tmp = "";

    num = 0;
    lastIndx = 0;
    tmp.append(*b);
    *b = "";
    tmp.append(buffer);
    num = tmp.find(DEL);
    while (num != -1)
    {
        array->push_back(tmp.substr(lastIndx, num - lastIndx + DELSIZE + 1));
        lastIndx = num + DELSIZE;
        num = tmp.find(DEL, lastIndx);
    }
    if (lastIndx != 0 && lastIndx + 1 != tmp.size())
        b->append(tmp.substr(lastIndx, tmp.size() - lastIndx));
}

int Server::ft_exec_communication_commands(int flag, std::string text, Client *client)
{
    Message message;
    std::string commands[5] = {
        "CAP",
        "NICK",
        "PASS",
        "PING",
        "USER"
    };
    int i;
    int num;

    i = 0;
    message.setMessage(text);
    while (i < 5 && message.getCommand().compare(commands[i]))
        i++;
    switch (i)
    {
        case (0):                                   //CAP
            return (flag);
            /*
            if ((flag & 4) == 0 || (flag & 4) == 4)
            {
                num = execCap(message, client, this);
                if (num == 0)
                    return (flag | 4);
                else if (num == 2 && (flag & 3) == 3)
                    return (7);
            }
            */
        case (1):                                   //NICK
            if (execNick(message, client, this))
                return (-1);
            return (flag | 2);
        case (2):                                    //PASS
            execPass(message, client, this);
            return (flag);
        case (3):
            execPing(message, client, this);
            return (flag);
        case (4):                                    //USER
            num = execUser(message, client);
            if (num == 1)
                return (flag | 1);
            if (num == -1)
                return (-1);
    }
    return (flag);
}

int Server::ft_welcome(Client *client)
{
    std::string text;

    text = makeWelcome(client->getNickname(), client->getUsername(), this->getServername());
    text.append(makeYourHost(this->getServername(), this->getVersion(), client->getNickname()));
    text.append(makeCreated(this->getDate(), client->getNickname()));
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return(0);
}
