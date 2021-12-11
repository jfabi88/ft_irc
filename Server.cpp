#include "Server.hpp"

/**CONSTRUCTOR**/

Server::Server()
{
    this->date = this->ft_set_date();
    std::cout << "Server created" << std::endl;
}

Server::Server(int port, int fd)
{
    this->port = port;
    this->fd = fd;
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

Client  Server::getClient(int indx) const
{
    return (this->clients[indx]);
}
std::string Server::getDate() const
{
    return (this->date);
}

Client  *Server::getClient(std::string name) const
{
    std::vector<Client>::const_iterator it;

    for (it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if ((*it).getNickname() == name)
        {
            return (new Client(*it));
        }
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

void    Server::setClient(Client newclient)
{
    this->clients.push_back(newclient);
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
    ret.append(" UTC");
    return (ret);
}
