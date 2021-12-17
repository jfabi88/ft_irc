#include "Cap.hpp"

Cap::Cap()
{
    this->command = "CAP";
    std::cout << "Cap created" << std::endl;
}

Cap::~Cap()
{
    std::cout << "Cap deleted" << std::endl;
}

int Cap::exec(Message message, Client *client, Server server)
{
    std::string subcommand;

    subcommand = message.getParametersIndex(0);
    std::cout << subcommand << std::endl;
    if (subcommand == "LIST")
        return (this->execList(client, server));
    else if (subcommand == "REQ")
        return (this->execReq(message, client, server));
    else if (subcommand == "LS")
        return (this->execList(client, server));
    else if (subcommand == "END")
        return (2);
    return (1);
}

/**PRIVATE-FUNCTIONS**/

std::string Cap::setAnswer(std::string text, Client client) const
{
    std::string ret;

    ret = ":" + client.getNickname() + " " + text;
    return (ret);
}

int Cap::execList(Client *client, Server server)
{
    std::string text;
    std::vector<std::string>    capabilities;
    std::vector<std::string>::iterator it;

    capabilities = server.getCapabilities();
    text = "CAP * LIST :";
    for (it = capabilities.begin(); it != capabilities.end(); it++)
    {
        text.append(*it);
        if (it + 1 == capabilities.end())
            text.append(DEL);
        else
            text.append(" ");
    }
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}

int Cap::execReq(Message message, Client *client, Server server)
{
    std::vector<std::string>    lastPrefix;

    lastPrefix = message.getLastParameterMatrix();
    std::vector<std::string>::iterator it;
    it = lastPrefix.begin();
    std::cout << *it << std::endl;
    if (!server.hasCapabilities(lastPrefix))
        this->execNak(message, client);
    else
        this->execAck(message, client);
    return (0);
}

int Cap::execNak(Message message, Client *client)
{
    std::string text;

    std::cout << "PIZZA" << std::endl;
    text = "CAP * NAK :";
    text.append(message.getLastParameter());
    text.append(DEL);
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}

int Cap::execAck(Message message, Client *client)
{
    std::vector<std::string>    lastPrefix;
    std::vector<std::string>    clientCap;
    std::vector<std::string>    tmp;
    std::string text;

    clientCap = client->getCapabilities();
    lastPrefix = message.getLastParameterMatrix();
    for(std::vector<std::string>::iterator it = lastPrefix.begin(); it != lastPrefix.end(); it++)
    {
        if ((*it)[0] != '-' && client->hasCapability(*it) == -1)
            tmp.push_back(*it);
        else if (((*it)[0] == '-' && client->hasCapability(*it) != -1))
            tmp.erase(tmp.begin() + client->hasCapability(*it));
    }
    text = "CAP * ACK :";
    text.append(message.getLastParameter());
    text.append(DEL);
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    client->setCapabilities(tmp);
    return (0);
}
