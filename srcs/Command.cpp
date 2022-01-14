/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/28 15:20:18 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/28 15:20:35 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#define USERLEN 16
#define CHANLIMIT 2

int execAway(Message message, Client *client)
{
    std::string text;

    if (message.getSize() < 1)
        return (0);
    if (client->getAway() && message.getParametersIndex(0) == "")
    {
        client->setAway(false, message.getParametersIndex(0));
        RepliesCreator reply;
        text = reply.makeUnAway(client->getUsername());
        send(client->getSocketFd(), text.c_str(), text.size(), 0);
    }
    else if (!client->getAway() && message.getParametersIndex(0) != "")
    {
        client->setAway(true, message.getParametersIndex(0));
        RepliesCreator reply;
        text = reply.makeNowAway(client->getUsername());
        send(client->getSocketFd(), text.c_str(), text.size(), 0);
    }
    return (0);
}

/***************CAP**********************/
static int execList(Client *client, Server *server)
{
    std::string text;
    std::vector<std::string>    capabilities;
    std::vector<std::string>::iterator it;

    capabilities = server->getCapabilities();
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

static int execNak(Message message, Client *client)
{
    std::string text;

    std::cout << "PIZZA" << std::endl;
    text = "CAP * NAK :";
    text.append(message.getLastParameter());
    text.append(DEL);
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}

static int execAck(Message message, Client *client)
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

static int execReq(Message message, Client *client, Server *server)
{
    std::vector<std::string>    lastPrefix;

    lastPrefix = message.getLastParameterMatrix();
    std::vector<std::string>::iterator it;
    it = lastPrefix.begin();
    std::cout << *it << std::endl;
    if (!server->hasCapabilities(lastPrefix))
        execNak(message, client);
    else
        execAck(message, client);
    return (0);
}

int execCap(Message message, Client *client, Server *server)
{
    std::string subcommand;

    if (message.getSize() > 0)
        subcommand = message.getParametersIndex(0);
    else
        return (0);
    if (subcommand == "LIST")
        return (execList(client, server));
    else if (subcommand == "REQ")
        return (execReq(message, client, server));
    else if (subcommand == "LS")
        return (execList(client, server));
    else if (subcommand == "END")
        return (2);
    return (1);
}
/****************************************************************/

int execNick(Message message, Client *client, Server *server)
{
    RepliesCreator  reply;
    std::string     nick;
    std::string     cNick;
    std::string     error;
    std::string     banCharacters = "?";
    int             i;

    error = "";
    nick = message.getLastParameter();
    cNick = client->getNickname();
    if (nick == "")
        error = reply.makeErrorNoNickNameGiven(cNick);
    else if (server->findClient(nick) != -1)
        error = reply.makeErrorNickNameInUse(cNick, nick);
    for (i = 0; i < nick.size(); i++)
    {
        if (banCharacters.find(nick[i]) != -1)
            error = reply.makeErrorErroneusNickName(cNick, nick);
    }
    if (error == "")
        client->setNickname(nick);
    else
        send(client->getSocketFd(), error.c_str(), error.size() + 1, 0);
    return (0);
}

//*******JOIN**********//

static int ft_parse_channel_key(Message message, std::vector<std::string> *channels, std::vector<std::string> *key)
{
    std::vector<std::string>::iterator it;
    std::vector<std::string>::const_iterator tmp;
    int size;
    int i;

    it = message.getParameters().begin();
    tmp = message.getLastParameterMatrix().begin();
    size = message.getLastParameterMatrix().size();
    i = 0;
    while (it < message.getParameters().end())
    {
        (*channels).push_back(*it);
        if (i < size)
        {
            (*key).push_back(*tmp);
            tmp++;
        }
        it++;
        i++;
    }
    return (0);
}

static std::string ft_success_join(Channel channel, Client client)
{
    RepliesCreator  reply;
    std::string     text;

    text = ":" + client.getNickname() + " JOIN " + channel.getName() + DEL;
    if (channel.getTopic() != "")
        text.append(reply.makeTopic(channel.getName(), channel.getTopic(), client.getNickname()));
    text.append(reply.makeNamReply(channel, client.getNickname()));
    text.append(reply.makeEndOfNames(channel.getName(), client.getNickname()));
    return (text);
}

static std::string ft_exec_join(std::string channelName, std::string key, Client *client, Server *server)
{
    RepliesCreator reply;
    Channel *newChannel;

    if (client->getChannelSub() >= CHANLIMIT)
        return (reply.makeTooManyChannels(client->getNickname(), channelName));
    newChannel = server->getChannel(channelName);
    if (newChannel == NULL)
    {
        try
        {
            newChannel = new Channel(channelName, key, client);
            client->addChannel(newChannel);
            server->addChannel(newChannel);
            return (ft_success_join(*newChannel, *client));
        }
        catch (const std::exception& e)
        {
            return (reply.makeErrorBadChanMask(channelName));
        }
    }
    else
    {
        int num;
        if (newChannel->isBanned(client->getNickname(), client->getUsername()))
            return (reply.makeErrorBannedFromChan(client->getNickname(), newChannel->getName()));
        else if (newChannel->hasMode("+i"))
            return (reply.makeInviteOnlyChan(client->getNickname(), newChannel->getName()));
        num = newChannel->addClient(client, key, 0, 0);
        if (num == 1)
            return (reply.makeErrorBadChannelKey(client->getNickname(), newChannel->getName()));
        else if (num == 2)
            return (reply.makeErrorChannelIsFull(client->getNickname(), newChannel->getName()));
        return (ft_success_join(*newChannel, *client));
    }
    return ("");
}

int execJoin(Message message, Client *client, Server *server)
{
    RepliesCreator  reply;
    std::string     text;
    std::vector<std::string> listChannel;
    std::vector<std::string> listKey;

    if (ft_parse_channel_key(message, &listChannel, &listKey))
    {
        text = reply.makeErrorNeedMoreParams(client->getNickname(), message.getCommand());
        send(client->getSocketFd(), text.c_str(), text.size(), 0);
    }
    else
    {
        std::vector<std::string>::iterator it;
        std::vector<std::string>::iterator keyIt;

        keyIt = listKey.begin();
        for (it = listChannel.begin(); it < listChannel.end(); it++)
        {
            text = ft_exec_join(*it, *keyIt, client, server);
            send(client->getSocketFd(), text.c_str(), text.size(), 0);
            keyIt++;
        }
    }
    return (0);
}
//***************************//

int execNotice(Message message, Client *client, Server *server)
{
    std::string target;
    Client      *clientTarget;
    RepliesCreator reply;
    std::string text;

    if (message.getSize() < 2)
        return (0);
    target = message.getParametersIndex(0);
    clientTarget = server->getClient(target);
    if (clientTarget == NULL)
        return (0);
    else if (clientTarget->getAway())
        return (0);
    else
    {
        text = ":" + client->getNickname() + " NOTICE " + clientTarget->getNickname() + " " + message.getLastParameter() + DEL;
        send(clientTarget->getSocketFd(), text.c_str(), text.size(), 0);
    }
    return (0);   
}

int execPart(Message message, Client *client, Server *server)
{
    std::vector<std::string>::iterator it;
    std::string channelName;
    std::string toSend;
    Channel *channel;
    RepliesCreator reply;

    for (it = message.getParameters().begin(); it <= message.getParameters().end(); it++)
    {
        channelName = *it;
        channel = server->getChannel(channelName);
        if (channel == NULL)
            toSend = reply.makeErrorNoSuchChannel(client->getNickname(), channelName);
        else if (!channel->getClient(client->getNickname()))
            toSend = reply.makeErrorNotOnChannel(client->getNickname(), channelName);
        else
        {
            if (channel->removeClient(client->getNickname()) == -1)
                server->removeChannel(channelName);
            toSend = ":" + client->getNickname() + " PART " + channelName + DEL;
        }
        send(client->getSocketFd(), toSend.c_str(), toSend.size(), 0);
    }
    return (0);
}

int execPass(Message message, Client *client, Server *server)
{
    RepliesCreator  reply;
    std::string     cNick;
    std::string     text = "";
    int             flag;

    cNick = client->getNickname();
    if (client->getRegistered())
        text = reply.makeErrorAlreadyRegistered(cNick);
    else if (message.getSize() < 1)
        text = reply.makeErrorNeedMoreParams(cNick, "PASS");
    else
    {
        flag = server->verifyPassword(message.getParametersIndex(0));
        client->setAccess(flag);
        if (flag != 1)
            text = reply.makePasswdMisMatch(cNick);
    }
    if (text != "")
        send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}

int execRPing(Message message, Client *client)
{
    RepliesCreator  reply;
    std::string     text;

    if (message.getSize() < 1)
        reply.makeErrorNeedMoreParams(client->getNickname(), message.getCommand());
    else
    {
        text.append("PONG ");
        text.append(message.getParametersIndex(0));
        send(client->getSocketFd(), text.c_str(), text.size(), 0);
        return (1);
    }
    return (0);
}

int execSPing(Message message, Client *client)
{
    std::string text;

    if (message.getSize() < 1)
        return (0);
    text = "PONG " + message.getParametersIndex(0) + DEL;
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    std::cout << "ciaone" << std::endl; 
    return (0);
}

//**********PRIVMSG***********************//

static int execPrivmsgChannel(Message message, Client *client, Server *server, std::string target)
{
    RepliesCreator  reply;
    Channel *channel;
    std::string text;
    std::vector<t_PChannel>::const_iterator it;

    channel = server->getChannel(target);
    if (channel == NULL)
    {
        text = reply.makeCannotSendToChan(client->getNickname(), target);
        send(client->getSocketFd(), text.c_str(), text.size(), 0);
    }
    else
    {
        for (it = channel->getFirstClient(); it < channel->getLastClient(); it++)
        {
            text = ":" + client->getNickname() +  " PRIVMSG " +  target + " :" + message.getLastParameter() + DEL;
            send((*it).client->getSocketFd(), text.c_str(), text.size(), 0);
        }
    }
    return (0);
}

static int execPrivmsgClient(Message message, Client *client, Server *server, std::string target)
{
    Client      *clientTarget;
    RepliesCreator reply;
    std::string text;

    clientTarget = server->getClient(target);
    if (clientTarget == NULL)
    {
        text = reply.makeNoSuchNick(target, 0);
        send(client->getSocketFd(), text.c_str(), text.size(), 0);
    }
    else if (clientTarget->getAway())
    {
        text = reply.makeAway(clientTarget->getUsername(), clientTarget->getNickname(), clientTarget->getAwayMessage());
        send(client->getSocketFd(), text.c_str(), text.size(), 0);
    }
    else
    {
        text = ":" + client->getNickname() +  " PRIVMSG " + clientTarget->getNickname() + " :" + message.getLastParameter() + DEL;
        send(clientTarget->getSocketFd(), text.c_str(), text.size(), 0);
    }
    return (0);
}

int execPrivmsg(Message message, Client *client, Server *server)
{
    std::string target;
    int         size;
    int         i;

    i = 0;
    size = message.getSize();
    if (size < 2)
        return (0);
    while (i < size - 1)
    {
        target = message.getParametersIndex(i);
        if (target != "" && (target[0] == '#' || target[0] == '&'))
            execPrivmsgChannel(message, client, server, target);
        else
            execPrivmsgClient(message, client, server, target);
        i++;
    }
    return (0);
}
//*******************************************************************//

int execUser(Message message, Client *client)
{
    RepliesCreator  reply;
    std::string     username;
    std::string     cNick;
    std::string     error;

    cNick = client->getNickname();
    if (message.getSize() < 4)
        error = reply.makeErrorNeedMoreParams(cNick, "USER");
    else if (client->getUsername() != "")
        error = reply.makeErrorAlreadyRegistered(cNick);
    else
    {
        username = message.getParametersIndex(0);
        if (username.size() > USERLEN)
            client->setUsername(username.substr(0, USERLEN));
        else
            client->setUsername(username);
        client->setRealname(message.getParametersIndex(3));
        return (1);
    }
    send(client->getSocketFd(), error.c_str(), error.size(), 0);
    return (0);
}

std::string listCommands[4] = {
    "AWAY",
    "JOIN",
    "NOTICE",
    "PRIVMSG"
};

int execCommand(Message message, Client *client, Server *server)
{
    int i = 0;
    std::string command = message.getCommand();

    while (i < listCommands->size() && listCommands[i].compare(command))
        i++;
    switch (i)
    {
        case 0:
            return (execAway(message, client)); //AWAY
        case 1:
            return (execJoin(message, client, server));              //JOIN
        case 2:
            return (execNotice(message, client, server));            //NOTICE
        case 3:
            return (execPrivmsg(message, client, server));         //PRIVMSG
        default:
            return (0);
    }
}
