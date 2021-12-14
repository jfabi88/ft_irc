#include "Cap.hpp"

Cap::Cap(Message newmessage, Server newserver, Client newclient) : ICommand(newmessage, newserver, newclient)
{
    std::cout << "Cap created" << std::endl;
}

Cap::~Cap()
{
    std::cout << "Cap deleted" << std::endl;
}

void Cap::exec() const
{
    std::string target;
    Client      *clientTarget;
    std::string text;

    target = this->message.getParametersIndex(0);
    clientTarget = this->server.getClient(target);
    if (clientTarget == NULL)
        return ;
    text = this->setAnswer(this->message.getText(), this->client);
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

void Cap::execList()
{
    std::string text;
    std::vector<std::string>    capabilities;
    std::vector<std::string>::iterator it;

    capabilities = this->server.getCapabilities();
    text = "CAP * LIST :";
    for (it = capabilities.begin(); it != capabilities.end(); it++)
    {
        text.append(*it);
        if (it + 1 == capabilities.end())
            text.append("\r\n");
        else
            text.append(" ");
    }
    send(this->client.getSocketFd(), text.c_str(), text.size(), 0);
}

void Cap::execReq()
{
    std::vector<std::string>    lastPrefix;

    lastPrefix = this->message.getLastParameterMatrix();
    if (!this->server.hasCapabilities(lastPrefix))
        this->execNak();
    else
        this->execAck();
}

void Cap::execNak()
{
    std::string text;

    text = "CAP * NAK :";
    text.append(this->message.getLastParameter());
    text.append("\r\n");
    send(this->client.getSocketFd(), text.c_str(), text.size(), 0);
}

void Cap::execAck()
{
    std::vector<std::string>    lastPrefix;
    std::vector<std::string>    clientCap;
    std::vector<std::string>    tmp;
    std::string text;

    clientCap = this->client.getCapabilities();
    lastPrefix = this->message.getLastParameterMatrix();
    for(std::vector<std::string>::iterator it = lastPrefix.begin(); it != lastPrefix.end(); it++)
    {
        if ((*it)[0] != '-' && client.hasCapability(*it) == -1)
            tmp.push_back(*it);
        else if (((*it)[0] == '-' && client.hasCapability(*it) != -1))
            tmp.erase(tmp.begin() + client.hasCapability(*it));
    }
    text = "CAP * ACK :";
    text.append(this->message.getLastParameter());
    text.append("\r\n");
    send(this->client.getSocketFd(), text.c_str(), text.size(), 0);
    this->client.setCapabilities(tmp);
}
