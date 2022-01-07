#include "Command.hpp"
#include "Server.hpp"

int execInvite(Message message, Client *client, Server *server)
{
    std::string     text;
    std::string     channelName;
    std::string     cNick;
    std::string     cTargetNick;
    Channel         *channel;
    RepliesCreator  reply;

    cNick = client->getNickname();
    if (message.getSize() < 2)
        text = reply.makeErrorNeedMoreParams(cNick, message.getCommand());
    else
    {
        cTargetNick = message.getParametersIndex(0);
        channelName = message.getParametersIndex(1);
        channel = server->getChannel(channelName);
        if (server->getClient(cTargetNick) == NULL)
            text = reply.makeNoSuchNick(cTargetNick, 0);
        else if (channel == NULL)
            text  = reply.makeErrorNoSuchChannel(cNick, channelName);
        else if (!channel->getClient(cNick))
            text = reply.makeErrorNotOnChannel(cNick, channelName);
        else if (channel->hasMode("+i") && channel->getT_PChannel(cNick).prefix != '@')
            text = reply.makeChanNoPrivsNeeded(cNick, channelName);
        else if (channel->getClient(cTargetNick))
            text = reply.makeErrorUserOnChannel(cNick, cTargetNick, channelName);
        else
        {
            text = cNick + " INVITE " + cTargetNick + " " + channelName + DEL;
            send(server->getClient(cTargetNick)->getSocketFd(), text.c_str(), text.size(), 0);
            text = reply.makeInviting(cNick, cTargetNick, channelName);
        }
    }
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}