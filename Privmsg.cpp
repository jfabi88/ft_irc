#include "Privmsg.hpp"

Privmsg::Privmsg(Message newmessage, Server newserver, Client newclient)
{
    this->message = newmessage;
    this->server = newserver;
    this->client = newclient;
    std::cout << "Privmsg created" << std::endl;
}

Privmsg::~Privmsg()
{
    std::cout << "Privmsg deleted" << std::endl;
}

void Privmsg::exec() const
{
    std::string target;
    std::string text;
    Client      *clientTarget;

    target = this->message.getParametersIndex(0);
    text = this->message.getParametersIndex(1);
    clientTarget = this->server.getClient(target);
    if (clientTarget == NULL)
        return ;
    //else if (clientTarget->getAway())
        //sent reply;
    else
        send(clientTarget->getSocketFd(), text.c_str(), text.size(), 0);
    delete clientTarget;
}