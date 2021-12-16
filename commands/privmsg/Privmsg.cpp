#include "Privmsg.hpp"

Privmsg::Privmsg()
{
    this->command = "PRIVMSG";
    std::cout << "Privmsg created" << std::endl;
}

Privmsg::~Privmsg()
{
    std::cout << "Privmsg deleted" << std::endl;
}

void Privmsg::exec(Message message, Client client, Server server)
{
    std::string target;
    Client      *clientTarget;
    std::string text;

    target = message.getParametersIndex(0);
    clientTarget = server.getClient(target);
    if (clientTarget == NULL)
        return ;
    text = this->setAnswer(message.getText(), client);
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
