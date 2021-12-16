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

void Cap::exec(Message message, Client client, Server server)
{
    std::string target;
    Client      *clientTarget;
    std::string text;

    target = message.getParametersIndex(0);
    clientTarget = server.getClient(target);
    if (clientTarget == NULL)
        return ;
    text = setAnswer(message.getText(), client);
    //if (clientTarget->getAway())
        //sent reply;
    //else
        send(clientTarget->getSocketFd(), text.c_str(), text.size(), 0);
    delete clientTarget;
}

/**PRIVATE-FUNCTIONS**/

std::string Cap::setAnswer(std::string text, Client client) const
{
    std::string ret;

    ret = ":" + client.getNickname() + " " + text;
    return (ret);
}

void Cap::execList(Client client, Server server)
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
            text.append("\r\n");
        else
            text.append(" ");
    }
    send(client.getSocketFd(), text.c_str(), text.size(), 0);
}

void Cap::execReq(Message message, Client client, Server server)
{
    std::vector<std::string>    lastPrefix;

    lastPrefix = message.getLastParameterMatrix();
    if (!server.hasCapabilities(lastPrefix))
        this->execNak(message, client);
    else
        this->execAck(message, client);
}

void Cap::execNak(Message message, Client client)
{
    std::string text;

    text = "CAP * NAK :";
    text.append(message.getLastParameter());
    text.append("\r\n");
    send(client.getSocketFd(), text.c_str(), text.size(), 0);
}

void Cap::execAck(Message message, Client client)
{
    std::vector<std::string>    lastPrefix;
    std::vector<std::string>    clientCap;
    std::vector<std::string>    tmp;
    std::string text;

    clientCap = client.getCapabilities();
    lastPrefix = message.getLastParameterMatrix();
    for(std::vector<std::string>::iterator it = lastPrefix.begin(); it != lastPrefix.end(); it++)
    {
        if ((*it)[0] != '-' && client.hasCapability(*it) == -1)
            tmp.push_back(*it);
        else if (((*it)[0] == '-' && client.hasCapability(*it) != -1))
            tmp.erase(tmp.begin() + client.hasCapability(*it));
    }
    text = "CAP * ACK :";
    text.append(message.getLastParameter());
    text.append("\r\n");
    send(client.getSocketFd(), text.c_str(), text.size(), 0);
    client.setCapabilities(tmp);
}
