#include "Server.hpp"

/**CONSTRUCTOR**/

Server::Server()
{
    this->servername = "IRC1ONE";
    this->date = this->ft_set_date();
    std::cout << "Server created" << std::endl;
}

Server::Server(int port, int fd)
{
    this->port = port;
    this->fd = fd;
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

Client  *Server::getClient(int indx) const
{
    return (this->clients[indx]);
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
