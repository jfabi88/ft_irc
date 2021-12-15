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

void    Server::setClient(Client newclient)
{
    this->clients.push_back(newclient);
}

/**PUBLIC-FUNCTIONS**/

int Server::hasCapability(std::string name) const
{
    std::vector<std::string>::const_iterator  it;
    std::string tmp;

    it = this->capabilities.begin();
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
            return (0);
    }
    return (1);
}

int     Server::findClient(std::string nickname) const
{
    std::vector<Client>::const_iterator it;
    int i;

    i = 0;
    it = this->clients.begin();
    for (it; it != this->clients.end(); it++)
    {
        if ((*it).getNickname().compare(nickname))
            return (i);
        i++;
    }
    return (-1);
}

int     Server::startCommunication(int fdNewClient)
{
    int flag;
    Client  client;
    Message message;
    RepliesCreator  reply;
    CommandCreator  cCreator;
    std::vector<std::string> array;

    flag = 1;
    while (flag)
    {
        array = this->ft_take_messages(fdNewClient);
        for (std::vector<std::string>::iterator it = array.begin();it != array.end(); it++)
        {
            message.setMessage(*it);
            ICommand *command = cCreator.makeCommand(message, *this, client);
            if (command)
            {
                if (command->getCommand().compare("PASS") && flag == 2)
                    reply.makeErrorAlreadyRegistered(client);
                else if (command->getCommand().compare("NICK"))
                    flag = 2;
                else if (command->getCommand().compare(""))
            }
        }
    }
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

std::vector<std::string> Server::ft_take_messages(int fdNewClient)
{
    char *buffer = new char[512]();
    std::string b;
    std::vector<std::string> array;

    b = "";
    recv(fdNewClient, buffer, 512, 0);
    this->ft_parse_data(&array, &b, buffer);
    while (b != "")
    {
        recv(fdNewClient, buffer, 512, 0);
        this->ft_parse_data(&array, &b, buffer);        
    }
    delete [] buffer;
    return (array);
}

void Server::ft_parse_data(std::vector<std::string> *array, std::string *b, char *buffer)
{
    int num;
    int lastIndx;
    std::string tmp;

    num = 0;
    lastIndx = 0;
    tmp.append(*b);
    tmp.append(buffer);
    num = tmp.find("\r\n");
    while (num != -1)
    {
        array->push_back(tmp.substr(lastIndx, num - lastIndx + 2));
        lastIndx = num + 2;
        num = tmp.find("\r\n");
    }
    if (lastIndx != 0)
        *b = "";
    if (lastIndx != tmp.size())
        b->append(tmp.substr(lastIndx, tmp.size() - lastIndx));
}
