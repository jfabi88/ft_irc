#include "Privmsg.hpp"

Privmsg::Privmsg(Message newmessage, Server newserver, Client newclient) : ICommand(newmessage, newserver, newclient)
{
    std::cout << "Privmsg created" << std::endl;
}

Privmsg::~Privmsg()
{
    std::cout << "Privmsg deleted" << std::endl;
}

void Privmsg::exec()
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

std::string Privmsg::setAnswer(std::string text, Client client) const
{
    std::string ret;

    ret = ":" + client.getNickname() + " " + text;
    return (ret);
}
