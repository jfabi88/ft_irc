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

int execAdmin(Message message, Client *client, Server *server)
{
    std::string text;

    if (message.getSize() > 0)
    {
        if (server->getClient(message.getParametersIndex(0)) == NULL)
            text = makeNoSuchServer(client->getNickname(), message.getParametersIndex(0));
    }
    else
    {
        text = makeAdminMe(client->getNickname());
        text.append(makeAdminLoc1(client->getNickname()));
        text.append(makeAdminLoc2(client->getNickname()));
        text.append(makeAdminEmail(client->getNickname()));
    }
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}

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
static int execCapList(Client *client, Server *server)
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
        return (execCapList(client, server));
    else if (subcommand == "REQ")
        return (execReq(message, client, server));
    else if (subcommand == "LS")
        return (execCapList(client, server));
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
        else if (channel->hasMode('i') && !channel->clientHasMode(cNick, 'o'))
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

int execInfo(Client *client)
{
    std::string text;

    text = makeInfo(client->getNickname());
    text.append(makeEndOfInfo(client->getNickname()));
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}

/*********KICK**************/

static int sendKick(std::string sender, std::string kicked, std::string reason, Channel *channel)
{
    std::vector<std::pair<int, Client *> >::const_iterator it;
    std::string text;
    std::vector<std::pair<int, Client *> >::const_iterator end;

    text = ":" + sender + " KICK " + channel->getName() + " " + kicked;
    if (reason != "")
        text += " :" + reason;
    text += DEL;
    end = channel->getLastClient();
    for (it = channel->getFirstClient(); it < end; it++)
        send((*it).second->getSocketFd(), text.c_str(), text.size(), 0);
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
        else if (channel->clientHasMode(cNick, 'o'))
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

//*******JOIN**********//

static int ft_parse_channel_key(Message message, std::vector<std::string> *channels, std::vector<std::string> *key)
{
    std::vector<std::string>            v;
    std::vector<std::string>::const_iterator  it;
    std::vector<std::string>::iterator  tmp;
    int size = 0;
    int i;

    v = message.getLastParameterMatrix();
    it = message.getParametersBegin();
    tmp = v.begin();
    size = v.size();
    i = 0;
    while (it < message.getParametersEnd())
    {
        std::cout << "Dentro ft_parse_channel_key. Elemento: " << *it << std::endl;
        channels->push_back(*it);
        if (i < size)
        {
            key->push_back(*tmp);
            tmp++;
        }
        it++;
        i++;
    }
    std::cout << "Join finito: " << std::endl;
    return (0);
}

static std::string ft_success_join(Channel channel, Client client)
{
    std::string     text;

    text = ":" + client.getNickname() + " JOIN " + channel.getName() + DEL;
    if (channel.getTopic() != "")
        text.append(makeTopic(channel.getName(), channel.getTopic(), client.getNickname()));
    text.append(makeNamReply(channel, client.getNickname(), 0));
    text.append(makeEndOfNames(channel.getName(), client.getNickname()));
    return (text);
}

static std::string ft_exec_join(std::string channelName, std::string key, Client *client, Server *server)
{
    Channel *newChannel;

    std::cout << "Siamo dentro exec join" << std::endl;
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
        if (newChannel->isBanned(client->getNickname(), client->getUsername(), client->getRealname()))
            return (makeErrorBannedFromChan(client->getNickname(), newChannel->getName()));
        else if (newChannel->hasMode('i'))
            return (makeInviteOnlyChan(client->getNickname(), newChannel->getName()));
        num = newChannel->addClient(client, key, 0);
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

        std::cout << "Stiamo prima key begin" << std::endl;
        keyIt = listKey.begin();
        std::cout << "Stiamo dopo key begin" << std::endl;
        for (it = listChannel.begin(); it < listChannel.end(); it++)
        {
            std::cout << "Stiamo for" << std::endl;
            if (keyIt != listKey.end())
            {
                text = ft_exec_join(*it, *keyIt, client, server);
                keyIt++;
            }
            else
                text = ft_exec_join(*it, "", client, server);
            send(client->getSocketFd(), text.c_str(), text.size(), 0);
        }
    }
    return (0);
}
//***************************//

int execList(Message message, Client *client, Server *server)
{
    std::string text;

    if (message.getParametersIndex(0) == "")
    {
        std::vector<Channel *>::const_iterator it;
        for (it = server->getFirstChannel(); it < server->getLastChannel(); it++)
        {
            if (!(*it)->hasMode('s') || (*it)->isOnChannel(client->getNickname()))
                text += makeRplList(client->getNickname(), *(*it));
        }
    }
    else
    {
        std::vector<std::string>::iterator  it;
        for (it = message.getParameters().begin(); it < message.getParameters().end(); it++)
        {
            if (server->getChannel(*it))
                text += makeRplList(client->getNickname(), *server->getChannel(*it));
        }
    }
    text += makeRplListEnd(client->getNickname());
    return (send(client->getSocketFd(), text.c_str(), text.size(), 0));
}

int execNick(Message message, Client *client, Server *server)
{
    std::string     nick;
    std::string     cNick;
    std::string     error;
    std::string     banCharacters = "?";
    size_t          i;

    error = "";
    nick = message.getParametersIndex(0);
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

//***********MODE************//

static std::string  execSpecialFlag(Channel *channel, std::string param, int flag, char c)
{
    if (c == 'o')
    {
        if (!channel->isOnChannel(param))
            return (makeErrorNotOnChannel(param, channel->getName()));
        channel->setOperator(param, flag);
    }
    else if (c == 'b')
        channel->setBanMask(param, flag);
    else if (c == 'k')
        channel->setKey(param, flag);
    else if (c == 'v')
    {
        if (!channel->isOnChannel(param))
            return (makeErrorNotOnChannel(param, channel->getName()));
        channel->setModeratorPermission(param, flag);
    }
    return ("");
}

static std::string  execChannelModeAdd(Channel *channel, Client *client, Message message, std::string m)
{
    int flag = 0;
    int i = 0;
    int n = 2;
    std::string text;

    if (m[0] == '+' || m[0] == '-')
    {
        flag = (m[0] == '-');
        i++;
    }
    for (int u = i; u < (int)(m.size()) && u < 12; u++)
    {
        if (m[u] == 'p' || m[u] == 's' || m[u] == 'i' || m[u] == 't' || m[u] == 'n'|| m[u] == 'm')
            channel->setMode(m[u], flag);
        else if (m[u] == 'o' || m[u] == 'l' || m[u] == 'b' || m[u] == 'v'|| m[u] == 'k')
        {
            if (message.getParametersIndex(n) == "" && (!flag || m[u] == 'o' || m[u] == 'v'))
                return (makeErrorNeedMoreParams(client->getNickname(), "MODE"));
            text = execSpecialFlag(channel, message.getParametersIndex(n), flag, m[u]);
            if (text != "")
                return (text);
            if (!flag || m[u] == 'o' || m[u] == 'v')
                n++;
        }
        else
            return (makeErrorUnKnownMode(client->getNickname(), m[i]));
    }
    return (text);
}

static std::string  execChannelMode(Message message, Client *client, Server *server)
{
    std::string text;
    std::string CNick;
    Channel     *channel;

    CNick = client->getNickname();
    channel = server->getChannel(message.getParametersIndex(0));
    if (channel == NULL)
        return (makeErrorNoSuchChannel(CNick, message.getParametersIndex(0)));
    if (!channel->isOnChannel(CNick))
        return (makeErrorNotOnChannel(CNick, channel->getName()));
    if (!channel->clientHasMode(CNick, 'o'))
        return (makeChanNoPrivsNeeded(CNick, channel->getName()));
    if (message.getParametersIndex(1) == "")
        return (makeErrorNeedMoreParams(CNick, "MODE"));
    return (execChannelModeAdd(channel, client, message, message.getParametersIndex(1)));
}

static std::string execUserMode(Message message, Client *client)
{
    std::string name;
    std::string mode;
    int         i;
    int         flag;

    i = 0;
    name = message.getParametersIndex(0);
    if (client->getNickname().compare(name))
        return (makeErrorUsersDontmatch(client->getNickname()));
    mode = message.getParametersIndex(1);
    if (mode == "")
        return (makeRplUModeis(client));
    if (mode[0] == '+' || mode[0] == '-')
    {
        flag = mode[0] == '-';
        i++;
    }
    for (int u = i; u < (int)(mode.size()); u++)
    {
        if (mode[u] != 'o' && mode[u] != 'i' && mode[u] != 's' && mode[u] == 'w')
            return (makeErrorUModeUnknownFlag(client->getNickname()));
    }
    for (int u = i; u < (int)(mode.size()); u++)
        client->setMode(mode[u], flag);
    return ("MODE " + mode + DEL);
}

int execMode(Message message, Client *client, Server *server)
{
    std::string name;
    std::string text;

    name = message.getParametersIndex(0);
    if (name == "")
        text = makeErrorNeedMoreParams(client->getNickname(), "MODE");
    if (name[0] == '#')
        text = execChannelMode(message, client, server);
    else
        text = execUserMode(message, client);
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}
//***************************//

//**************MOTD********//
static std::string  makeRepliesMotD(std::string CNick, std::string Servername, std::string motd)
{
    std::string text;
    int         size;
    int         motdSize = motd.size();
    int         start = 0;
    int         end = 0;

    size = 29 + DELSIZE + CNick.size() + Servername.size();
    if (size + motdSize > 512)
        end = 512 - size;
    else
        end = motdSize;
    text = makeMotDStart(CNick, Servername, motd.substr(start, end));
    while (end != motdSize)
    {
        size = 6 + DELSIZE + CNick.size();
        start = end;
        if (size + motd.substr(start, motdSize).size() > 512)
            end = 512 - size + start;
        else
            end = motdSize;
        text.append(makeMotDStart(CNick, Servername, motd.substr(start, end)));
    }
    text.append(makeEndOfMotD(CNick));
    return (text);
}

int execMotd(Message message, Client *client, Server *server)
{
    std::string text;

    if (message.getSize() > 0)
        text = makeNoSuchServer(client->getNickname(), message.getParametersIndex(0));
    else if (server->getMotD() == "")
        text = makeErrorMotD(client->getNickname());
    else
        text = makeRepliesMotD(client->getNickname(), server->getServername(), server->getMotD());
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}
//************************************//

int execNames(Message message, Client *client, Server *server)
{
    std::string text;
    std::string channelName;
    Channel     *channel;

    channel = server->getChannel(channelName);
    channelName = message.getParametersIndex(0);
    if (channelName == "")
        text = makeEndOfNames("*", client->getNickname());
    else if (!channel)
        text = makeEndOfNames(channelName, client->getNickname());
    else if (channel->hasMode('s') && !channel->isOnChannel(client->getNickname()))
        text = makeEndOfNames(channelName, client->getNickname());
    else
    {
        text = makeNamReply(*channel, client->getNickname(), channel->isOnChannel(client->getNickname()));
        text += makeEndOfNames(channelName, client->getNickname());
    }
    return (send(client->getSocketFd(), text.c_str(), text.size(), 0));
}

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
    std::vector<std::pair<int, Client *> >::const_iterator it;

    text = "";
    channel = server->getChannel(target);
    if (channel == NULL)
        text = makeCannotSendToChan(client->getNickname(), target);
    else if (channel->isBanned(client->getNickname(), client->getUsername(), client->getRealname()))
        text = makeErrorBannedFromChan(client->getNickname(), channel->getName());
    else if (channel->hasMode('m') && (!channel->clientHasMode(client->getNickname(), 'v') && !channel->clientHasMode(client->getNickname(), 'v')))
        text = makeChanNoPrivsNeeded(client->getNickname(), channel->getName());
    if (text != "")
        return (send(client->getSocketFd(), text.c_str(), text.size(), 0));
    for (it = channel->getFirstClient(); it < channel->getLastClient(); it++)
    {
        text = ":" + client->getNickname() +  " PRIVMSG " +  target + " :" + message.getLastParameter() + DEL;
        send((*it).second->getSocketFd(), text.c_str(), text.size(), 0);
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

int execTopic(Message message, Client *client, Server *server)
{
    Channel     *channel;
    std::string cNick;
    std::string text;

    cNick = client->getNickname();
    if (message.getParametersIndex(0) == "")
        text = makeErrorNeedMoreParams(cNick, "TOPIC");
    else
    {
        channel = server->getChannel(message.getParametersIndex(0));
        if (channel == NULL)
            text = makeErrorNoSuchChannel(cNick, message.getParametersIndex(0));
        else if (!channel->isOnChannel(cNick))
            text = makeErrorNotOnChannel(cNick, channel->getName());
        else if (channel->hasMode('t') && !channel->clientHasMode(cNick, 'o'))
            text = makeChanNoPrivsNeeded(cNick, channel->getName());
        else
        {
            if (message.getIsLastParameter())
            {
                channel->setTopic(message.getParametersIndex(1));
                text = "TOPIC " + channel->getName() + " :" + channel->getTopic() + DEL;
                std::vector<std::pair<int, Client *> >::const_iterator it;
                for (it = channel->getFirstClient(); it < channel->getLastClient(); it++)
                {
                    if (send((*it).second->getSocketFd(), text.c_str(), text.size(), 0) == -1)
                        return (-1);
                }
                return (0);
            }
            else if (channel->getTopic() != "")
                text = makeTopic(channel->getName(), channel->getTopic(), client->getNickname());
            else
                text = makeNoTopic(channel->getName(), client->getNickname());
        }
    }
    return (send(client->getSocketFd(), text.c_str(), text.size(), 0));
}

int execTime(Message message, Client *client, Server *server)
{
    std::string text;

    if (message.getSize() > 0)
    {
        if (server->getClient(message.getParametersIndex(0)) == NULL)
            text = makeNoSuchServer(client->getNickname(), message.getParametersIndex(0));
    }
    else
        text = makeTime(client->getNickname(), server->getServername(), server->returnDate());
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
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

int execVersion(Message message, Client *client, Server *server)
{
    std::string text;

    if (message.getSize() > 0)
    {
        if (server->getClient(message.getParametersIndex(0)) == NULL)
            text = makeNoSuchServer(client->getNickname(), message.getParametersIndex(0));
    }
    else
    {
        text = makeVersion(client->getNickname());
        text.append(makeISupport(client->getNickname(), server->getParameter()));
    }
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}

//*****************WHO***********************//

static std::string execWhoNoParameter(Client *client, Server *server)
{
    std::string text;

    std::vector<Client *> listClient = server->getClients();
    for (std::vector<Client *>::iterator it = listClient.begin(); it < listClient.end(); it++)
    {
        if (*(client) != *(*it) && (*it)->hasMode('i') && server->haveChannelCommon(client, (*it)) == 0)
            text.append(makeWhoReply());
    }
    return (text);
}

static std::string execWhoChannel(std::string channelName, Server *server, int op)
{
    std::string text;

    Channel * channel = server->getChannel(channelName);
    if (channel)
    {
        for (std::vector<std::pair<int, Client *> >::const_iterator it = channel->getFirstClient(); it < channel->getLastClient(); it++)
        {
            if ((*it).second->hasMode('i') != 0)
            {
                if ((op == 1 && (*it).second->hasMode('i') == 1) || op == 0)
                    text.append(makeWhoReply());
            }
        }
    }
    return (text);
}

static std::string execWhoNick(std::string nick, Server *server, int op)
{
    std::string text;

    Client *whoClient = server->getClient(nick);
    if (whoClient && whoClient->hasMode('i'))
    {
        if ((op == 1 && whoClient->hasMode('o')) || op == 0)
            text.append(makeWhoReply());
    }
    return (text);
}

int execWho(Message message, Client *client, Server *server)
{
    std::string text;

    if (message.getSize() == 0)
        text.append(execWhoNoParameter(client, server));
    else if (message.getSize() == 1)
    {
        if (message.getParametersIndex(0)[0] == '&' || message.getParametersIndex(0)[0] == '#')
            text.append(execWhoChannel(message.getParametersIndex(0), server, 0));
        else
            text.append(execWhoNick(message.getParametersIndex(0), server, 0));
    }
    else if (message.getParametersIndex(1)[0] == 'o')
    {
        if (message.getParametersIndex(0)[0] == '&' || message.getParametersIndex(0)[0] == '#')
            text.append(execWhoChannel(message.getParametersIndex(0), server, 1));
        else
            text.append(execWhoNick(message.getParametersIndex(0), server, 1));
    }
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}
//****************************************//

std::string listCommands[20] = {
    "ADMIN",
    "AWAY",
    "INFO",
    "INVITE",
    "JOIN",
    "KICK",
    "LIST",
    "MODE",
    "MOTD",
    "NAMES",
    "NICK",
    "NOTICE",
    "PART",
    "PASS",
    "PRIVMSG",
    "QUIT",
    "TIME",
    "TOPIC",
    "USER",
    "VERSION"
};

int execCommand(Message message, Client *client, Server *server)
{
    size_t i = 0;
    std::string command = message.getCommand();

    while (i < sizeof(listCommands) / sizeof(std::string) && listCommands[i].compare(command))
        i++;
    switch (i)
    {
        case 0:
            return (execAdmin(message, client, server));
        case 1:
            return (execAway(message, client));
        case 2:
            return (execInfo(client));
        case 3:
            return (execInvite(message, client, server));
        case 4:
            return (execJoin(message, client, server));
        case 5:
            return (execKick(message, client, server));
        case 6:
            return (execList(message, client, server));
        case 7:
            return (execMode(message, client, server));
        case 8:
            return (execMotd(message, client, server));
        case 9:
            return (execNames(message, client, server));
        case 10:
            return (execNick(message, client, server));
        case 11:
            return (execNotice(message, client, server));
        case 12:
            return (execPart(message, client, server));
        case 13:
            return (execPass(message, client, server));
        case 14:
            return (execPrivmsg(message, client, server));
        case 15:
            return (execQuit(message, client, server));
        case 16:
            return (execTime(message, client, server));
        case 17:
            return (execTopic(message, client, server));
        case 18:
            return (execUser(message, client));
        case 19:
            return (execVersion(message, client, server));
        default:
            return (0);
    }
}
