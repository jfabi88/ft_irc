#include "Server.hpp"
#include "Command.hpp"

static int sendKick(std::string sender, std::string kicked, std::string reason, Channel *channel)
{
    std::vector<t_PChannel>::const_iterator it;
    std::string text;
    std::vector<t_PChannel>::const_iterator end;

    text = ":" + sender + " KICK " + channel->getName() + " " + kicked;
    if (reason != "")
        text += " :" + reason;
    text += DEL;
    end = channel->getLastClient();
    for (it = channel->getFirstClient(); it < end; it++)
        send((*it).client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}

int execKick(Message message, Client *client, Server *server)
{
    std::string     text;
    RepliesCreator  reply;
    std::string     channelName;
    std::string     cNick;
    std::string     cTargetNick;

    cNick = client->getNickname();
    if (message.getSize() < 2)
        text = reply.makeErrorNeedMoreParams(cNick, message.getCommand());
    else
    {
        Channel *channel = server->getChannel(channelName);
        channelName = message.getParametersIndex(0);
        if (!server->getClient(cTargetNick))
            text = reply.makeNoSuchNick(cTargetNick, 0);
        else if(!channel)
            text = reply.makeErrorNoSuchChannel(cNick, channelName);
        else if (!channel->getClient(cNick))
            text = reply.makeErrorNotOnChannel(cNick, channelName);
        else if (!channel->getClient(cTargetNick))
            text = reply.makeErrorNotOnChannel(cTargetNick, channelName);
        else if (channel->getT_PChannel(cNick).prefix != '@')
            text = reply.makeChanNoPrivsNeeded(cNick, channelName);
        else
        {
            Client *clientTarget = server->getClient(cTargetNick);
            channel->removeClient(cTargetNick);
            clientTarget->removeChannel(channelName);
            if (message.getSize() == 3)
                sendKick(cNick, cTargetNick, message.getLastParameter(), channel);
            else
                sendKick(cNick, cTargetNick, "", channel);
        }
    }
    return (0);
}
