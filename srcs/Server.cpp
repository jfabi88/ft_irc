#include "Server.hpp"

/**CONSTRUCTOR**/

Server::Server()
{
    this->servername = "IRC1ONE";
    this->date = this->ft_set_date();
    this->password = "";
    std::cout << "Server created" << std::endl;
}

Server::Server(int port, int fd, std::string password)
{
    this->port = port;
    this->fd = fd;
    std::cout << password << std::endl;
    this->password = password;
    this->date = this->ft_set_date();
    this->servername = "IRC1ONE";
}

/**GET-SET**/

int     Server::getPort() const
{
    return (this->port);
}

int     Server::getSocket() const
{
    return (this->fd);
}

std::string Server::getServername() const
{
    return (this->servername);
}

std::string Server::getVersion() const
{
    return (this->version);
}

std::vector<Client *> Server::getClients() const
{
    return (this->clients);
}

Client  *Server::getClient(int fd) const
{
    std::vector<Client *>::const_iterator it;

    for (it = this->clients.begin(); it != this->clients.end(); it++)
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
    return (this->date);
}

Client  *Server::getClient(std::string name) const
{
    std::vector<Client *>::const_iterator it;

    for (it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if ((*it)->getNickname() == name)
        {
            return (*it);
        }
    }
    return (NULL);
}

std::vector<std::string> Server::getCapabilities() const
{
    return (this->capabilities);
}

Channel *Server::getChannel(std::string name) const
{
    std::vector<Channel *>::const_iterator it;

    for (it = this->channels.begin(); it != this->channels.end() ;it++)
    {
        if (!(*it)->getName().compare(name))
            return (*it);
    }
    return (NULL);
}

void    Server::setPort(int newport)
{
    this->port = newport;
}

void    Server::setSocket(int newfd)
{
    this->fd = newfd;
}

void    Server::setServername(std::string newservername)
{
    this->servername = newservername;
}

void    Server::setVersion(std::string newversion)
{
    this->version = newversion;
}

void    Server::setClient(Client *newclient)
{
    this->clients.push_back(newclient);
}

int    Server::verifyPassword(std::string userPassword)
{
    if (this->password == "")
        return (1);
    else if (!userPassword.compare(this->password))
        return (1);
    return (0);
}

void    Server::addChannel(Channel *channel)
{
    this->channels.push_back(channel);
}

void    Server::removeChannel(std::string channelName)
{
    std::vector<Channel *>::iterator it;

    for (it = this->channels.begin(); it < this->channels.end(); it++)
    {
        if (!((*it)->getName().compare(channelName)))
        {
            this->channels.erase(it);
            delete (*it);
            return ;
        }
    }
}

void    Server::removeClient(std::string clientName)
{
    std::vector<Client *>::iterator it;

    for (it = this->clients.begin(); it < this->clients.end(); it++)
    {
        if (!((*it)->getNickname().compare(clientName)))
        {
            this->clients.erase(it);
            delete (*it);
            return ;
        }
    }    
}

/**PUBLIC-FUNCTIONS**/

int Server::hasCapability(std::string name) const
{
    std::vector<std::string>::const_iterator  it;
    std::string tmp;

    it = this->capabilities.begin();
    if (it == this->capabilities.end())
        return (0);
    std::cout << "COME VA" << std::endl;
    tmp = name;
    if (name != "" && name[0] == '-')
        tmp = name.substr(1, name.size());
    while (!(*it).compare(tmp) && it != this->capabilities.end())
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
        {
            std::cout << "AIO" << std::endl;
            return (0);
        }
    }
    return (1);
}

int     Server::findClient(std::string nickname) const
{
    std::vector<Client *>::const_iterator it;
    int i;

    i = 0;
    it = this->clients.begin();
    for (it = this->clients.begin(); it != this->clients.end(); it++)
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
            flag = this->ft_exec_communication_commands(flag, *it, client);
    }
    client->setRecFlag(flag);
    if (flag == 3 || flag == 7)
    {
        if (this->password != "" && !client->getAccess())
        {
            std::string text = makePasswdMisMatch(client->getNickname());
            send(fdNewClient, text.c_str(), text.size(), 0);
            delete client;
            return (0);
        }
        client->setRegistered(true);
        std::cout << *client << std::endl;
        ft_welcome(client);
    }
    return (0);
}

int     Server::receiveCommand(int fdClient, char *buffer)
{
    std::vector<std::string> array;
    Message message;
    Client  *client;

    client = this->getClient(fdClient);
    array = ft_take_messages(fdClient, buffer);
    for (std::vector<std::string>::iterator it = array.begin(); it != array.end(); it ++)
    {
        message.setMessage(*it);
        execCommand(message, client, this);
    }
    std::cout << "Messaggio eseguito" << std::endl;
    return (0);
}

void    Server::printClients()
{
    std::vector<Client *>::iterator it;

    for (it = this->getClients().begin(); it != this->getClients().end(); it++)
        std::cout << "Nick: " << (*it)->getNickname() << std::endl;
}

/**PRIVATE-FUNCTIONS**/

std::string Server::ft_set_date()
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

void Server::ft_memset(char *buffer, int size)
{
    for (int i = 0; i < size ;i++)
        buffer[i] = 0;
}

std::vector<std::string> Server::ft_take_messages(int fdClient, char *buffer)
{
    std::vector<std::string> array;
    std::string b;

    std::cout << "prima di recv" << std::endl;
    b = "";
    if (buffer[0] == 0)
        recv(fdClient, buffer, 512, 0);   //jfabi: a che serve questa cosa?
    std::cout << "dopo di recv. Il buffer: " << buffer << std::endl;
    this->ft_parse_data(&array, &b, buffer);
    ft_memset(buffer, 512);
    while (b != "")                        //jfabi: questo serve per il multilines, cosa che noi non dobbiamo gestire per forza
    {
        std::cout << "Prima di recv" << std::endl;
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
            execNick(message, client, this);
            return (flag | 2);
        case (2):                                    //PASS
            execPass(message, client, this);
            return (flag);
        case (3):
            execSPing(message, client);
            return (flag);
        case (4):                                    //USER
            num = execUser(message, client);
            if (num == 1)
                return (flag | 1);
    }
    return (flag);
}

int Server::ft_welcome(Client *client)
{
    std::string text;

    text = makeWelcome(client->getNickname(), client->getUsername(), this->getServername());
    text.append(makeYourHost(this->getServername(), this->getVersion(), client->getNickname()));
    text.append(makeCreated(this->getDate(), client->getNickname()));
    std::cout << text << std::endl;
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return(0);
}
