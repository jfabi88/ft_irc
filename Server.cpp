#include "Server.hpp"

/**CONSTRUCTOR**/

Server::Server()
{
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

Client  Server::getClient(int indx) const
{
    return (this->clients[indx]);
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

void    Server::setClient(Client newclient)
{
    this->clients.push_back(newclient);
}

