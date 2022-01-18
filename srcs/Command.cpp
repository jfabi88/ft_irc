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

int execAway(Message message, Client *client)
{
    std::string text;

    if (message.getSize() < 1)
        return (0);
    if (client->getAwayStatus() && message.getParametersIndex(0) == "")
    {
        client->setAway(false, message.getParametersIndex(0));
        text = makeUnAway(client->getUsername());
        send(client->getSocketFd(), text.c_str(), text.size(), 0);
    }
    else if (!client->getAwayStatus() && message.getParametersIndex(0) != "")
    {
        client->setAway(true, message.getParametersIndex(0));
        text = makeNowAway(client->getUsername());
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

int execInvite(Message message, Client *client, Server *server)
{
    std::string     text;
    std::string     channelName;
    std::string     cNick;
    std::string     cTargetNick;
    Channel         *channel;

    cNick = client->getNickname();
    if (message.getSize() < 2)
        text = makeErrorNeedMoreParams(cNick, message.getCommand());
    else
    {
        cTargetNick = message.getParametersIndex(0);
        channelName = message.getParametersIndex(1);
        channel = server->getChannel(channelName);
        if (server->getClient(cTargetNick) == NULL)
            text = makeNoSuchNick(cTargetNick, 0);
        else if (channel == NULL)
            text  = makeErrorNoSuchChannel(cNick, channelName);
        else if (!channel->getClient(cNick))
            text = makeErrorNotOnChannel(cNick, channelName);
        else if (channel->hasMode("+i") && channel->getT_PChannel(cNick).prefix != '@')
            text = makeChanNoPrivsNeeded(cNick, channelName);
        else if (channel->getClient(cTargetNick))
            text = makeErrorUserOnChannel(cNick, cTargetNick, channelName);
        else
        {
            text = cNick + " INVITE " + cTargetNick + " " + channelName + DEL;
            send(server->getClient(cTargetNick)->getSocketFd(), text.c_str(), text.size(), 0);
            text = makeInviting(cNick, cTargetNick, channelName);
        }
    }
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}

/*********KICK**************/

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
    std::string     channelName;
    std::string     cNick;
    std::string     cTargetNick;

    cNick = client->getNickname();
    if (message.getSize() < 2)
        text = makeErrorNeedMoreParams(cNick, message.getCommand());
    else
    {
        Channel *channel = server->getChannel(channelName);
        channelName = message.getParametersIndex(0);
        if (!server->getClient(cTargetNick))
            text = makeNoSuchNick(cTargetNick, 0);
        else if(!channel)
            text = makeErrorNoSuchChannel(cNick, channelName);
        else if (!channel->getClient(cNick))
            text = makeErrorNotOnChannel(cNick, channelName);
        else if (!channel->getClient(cTargetNick))
            text = makeErrorNotOnChannel(cTargetNick, channelName);
        else if (channel->getT_PChannel(cNick).prefix != '@')
            text = makeChanNoPrivsNeeded(cNick, channelName);
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
/********************************/

int execNick(Message message, Client *client, Server *server)
{
    std::string     nick;
    std::string     cNick;
    std::string     error;
    std::string     banCharacters = "?";
    size_t          i;

    error = "";
    nick = message.getLastParameter();
    cNick = client->getNickname();
    if (nick == "")
        error = makeErrorNoNickNameGiven(cNick);
    else if (server->findClient(nick) != -1)
        error = makeErrorNickNameInUse(cNick, nick);
    for (i = 0; i < nick.size(); i++)
    {
        if (banCharacters.find(nick[i]) != std::string::npos)
            error = makeErrorErroneusNickName(cNick, nick);
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
    std::string     text;

    text = ":" + client.getNickname() + " JOIN " + channel.getName() + DEL;
    if (channel.getTopic() != "")
        text.append(makeTopic(channel.getName(), channel.getTopic(), client.getNickname()));
    text.append(makeNamReply(channel, client.getNickname()));
    text.append(makeEndOfNames(channel.getName(), client.getNickname()));
    return (text);
}

static std::string ft_exec_join(std::string channelName, std::string key, Client *client, Server *server)
{
    Channel *newChannel;

    if (client->getChannelSub() >= CHANLIMIT)
        return (makeTooManyChannels(client->getNickname(), channelName));
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
            return (makeErrorBadChanMask(channelName));
        }
    }
    else
    {
        int num;
        if (newChannel->isBanned(client->getNickname(), client->getUsername()))
            return (makeErrorBannedFromChan(client->getNickname(), newChannel->getName()));
        else if (newChannel->hasMode("+i"))
            return (makeInviteOnlyChan(client->getNickname(), newChannel->getName()));
        num = newChannel->addClient(client, key, 0, 0);
        if (num == 1)
            return (makeErrorBadChannelKey(client->getNickname(), newChannel->getName()));
        else if (num == 2)
            return (makeErrorChannelIsFull(client->getNickname(), newChannel->getName()));
        return (ft_success_join(*newChannel, *client));
    }
    return ("");
}

int execJoin(Message message, Client *client, Server *server)
{
    std::string     text;
    std::vector<std::string> listChannel;
    std::vector<std::string> listKey;

    if (ft_parse_channel_key(message, &listChannel, &listKey))
    {
        text = makeErrorNeedMoreParams(client->getNickname(), message.getCommand());
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
    std::string text;

    if (message.getSize() < 2)
        return (0);
    target = message.getParametersIndex(0);
    clientTarget = server->getClient(target);
    if (clientTarget == NULL)
        return (0);
    else if (clientTarget->getAwayStatus())
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

    for (it = message.getParameters().begin(); it <= message.getParameters().end(); it++)
    {
        channelName = *it;
        channel = server->getChannel(channelName);
        if (channel == NULL)
            toSend = makeErrorNoSuchChannel(client->getNickname(), channelName);
        else if (!channel->getClient(client->getNickname()))
            toSend = makeErrorNotOnChannel(client->getNickname(), channelName);
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
    std::string     cNick;
    std::string     text = "";
    int             flag;

    cNick = client->getNickname();
    if (client->getRegisteredStatus())
        text = makeErrorAlreadyRegistered(cNick);
    else if (message.getSize() < 1)
        text = makeErrorNeedMoreParams(cNick, "PASS");
    else
    {
        flag = server->verifyPassword(message.getParametersIndex(0));
        client->setAccess(flag);
        if (flag != 1)
            text = makePasswdMisMatch(cNick);
    }
    if (text != "")
        send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}

int execRPing(Message message, Client *client)
{
    std::string     text;

    if (message.getSize() < 1)
        makeErrorNeedMoreParams(client->getNickname(), message.getCommand());
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
    Channel *channel;
    std::string text;
    std::vector<t_PChannel>::const_iterator it;

    channel = server->getChannel(target);
    if (channel == NULL)
    {
        text = makeCannotSendToChan(client->getNickname(), target);
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
    std::string text;

    clientTarget = server->getClient(target);
    if (clientTarget == NULL)
    {
        text = makeNoSuchNick(target, 0);
        send(client->getSocketFd(), text.c_str(), text.size(), 0);
    }
    else if (clientTarget->getAwayStatus())
    {
        text = makeAway(clientTarget->getUsername(), clientTarget->getNickname(), clientTarget->getAwayMessage());
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

int execUser(Message message, Client *client)
{
    std::string     username;
    std::string     cNick;
    std::string     error;

    cNick = client->getNickname();
    if (message.getSize() < 4)
        error = makeErrorNeedMoreParams(cNick, "USER");
    else if (client->getUsername() != "")
        error = makeErrorAlreadyRegistered(cNick);
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

std::string listCommands[7] = {
    "AWAY",
    "INVITE",
    "JOIN",
    "KICK",
    "NOTICE",
    "PRIVMSG",
    "QUIT"
};

int execCommand(Message message, Client *client, Server *server)
{
    size_t i = 0;
    std::string command = message.getCommand();

    while (i < listCommands->size() && listCommands[i].compare(command))
        i++;
    switch (i)
    {
        case 0:
            return (execAway(message, client)); //AWAY
        case 1:
            return (execInvite(message, client, server));           //INVITE
        case 2:
            return (execJoin(message, client, server));              //JOIN
        case 3:
            return (execKick(message, client, server));             //KICK
        case 4:
            return (execNotice(message, client, server));            //NOTICE
        case 5:
            return (execPrivmsg(message, client, server));         //PRIVMSG
        case 6:
            return (execQuit(message, client, server));            //QUIT
        default:
            return (0);
    }
}
