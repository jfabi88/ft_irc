#include "Command.hpp"
#include "Server.hpp"

int execQuit(Message message, Client *client, Server *server)
{
    std::string text;
    std::vector<Channel *>::const_iterator  it;
    std::vector<Channel *>::const_iterator  end;

    text = ":" + client->getNickname() + " QUIT";
    if (message.getSize() == 1)
        text += " :Quit:" + message.getParametersIndex(0);
    text += DEL;
    end = client->getLastChannel();
    for (it = client->getFirstChannel(); it < end; it++)
    {
        (*it)->removeClient(client->getSocketFd());
        (*it)->sendToAll(text);
    }
    server->removeClient(client->getNickname());
    return (0);
}
