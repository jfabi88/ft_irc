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
    std::cout << "Il testo inviato é: " << text << std::endl;
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}

int execAway(Message message, Client *client)
{
    std::string text;
 
    if (client->getAwayStatus() && message.getLastParameter() == "")
    {
        client->setAway(false, message.getLastParameter());
        text = makeUnAway(client->getUsername());
        std::cout << "Il testo inviato é: " << text << std::endl;
        send(client->getSocketFd(), text.c_str(), text.size(), 0);
    }
    else if (!client->getAwayStatus() && message.getLastParameter() != "")
    {
        client->setAway(true, message.getLastParameter());
        text = makeNowAway(client->getUsername());
        std::cout << "Il testo inviato é: " << text << std::endl;
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
    std::cout << "Il testo inviato é: " << text << std::endl;
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
    std::cout << "Il testo inviato é: " << text << std::endl;
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
    std::cout << "Il testo inviato é: " << text << std::endl;
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

int execInfo(Client *client, Server *server)
{
    std::string text;

    text = makeInfo(client->getNickname(), server->getDate());
    text.append(makeEndOfInfo(client->getNickname()));
    std::cout << "Il testo inviato é: " << text << std::endl;
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}

int execInvite(Message message, Client *client, Server *server)
{
    std::string     text;
    std::string     channelName;
    std::string     cNick;
    std::string     cTargetNick;
    Channel         *channel;
    Client          *clientTarget;

    cNick = client->getNickname();
    if (message.getSize() < 2)
        text = makeErrorNeedMoreParams(cNick, message.getCommand());
    else
    {
        cTargetNick = message.getParametersIndex(0);
        channelName = message.getParametersIndex(1);
        channel = server->getChannel(channelName);
        clientTarget = server->getClient(cTargetNick);
        if (clientTarget == NULL)
            text = makeNoSuchNick(cTargetNick, 0);
        else if (channel == NULL)
            text  = makeErrorNoSuchChannel(cNick, channelName);
        else if (!channel->getClient(cNick))
            text = makeErrorNotOnChannel(cNick, channelName);
        else if (channel->hasMode('i') && !channel->clientHasMode(cNick, 'o'))
            text = makeChanNoPrivsNeeded(cNick, channelName);
        else if (channel->getClient(cTargetNick))
            text = makeErrorUserOnChannel(cNick, cTargetNick, channelName);
        else if (channel->hasMode('l') && channel->getNClient() >= channel->getLimit())
            text = makeErrorChannelIsFull(client->getNickname(), channelName);
        else
        {
            channel->addClient(clientTarget, 0);
            clientTarget->addChannel(channel);
            text = ":" + cNick + " INVITE " + cTargetNick + " " + channelName + DEL;
            std::cout << "Il testo inviato é: " << text << std::endl;
            send(server->getClient(cTargetNick)->getSocketFd(), text.c_str(), text.size(), 0);
            text = makeInviting(cNick, cTargetNick, channelName);
        }
    }
    std::cout << "Il testo inviato é: " << text << std::endl;
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}

/*********KICK**************/

static std::string sendKick(std::string sender, std::string kicked, std::string reason, Channel *channel)
{
    std::string text;

    text = ":" + sender + " KICK " + channel->getName() + " " + kicked;
    if (reason != "")
        text += " :" + reason;
    text += DEL;
    return (text);
}

int execKick(Message message, Client *client, Server *server)
{
    std::string     text;
    std::string     channelName;
    std::string     cNick;
    std::string     cTargetNick;
    std::vector<std::pair<int, Client *> > cOnChan;

    cNick = client->getNickname();
    if (message.getSize() < 2)
        text = makeErrorNeedMoreParams(cNick, message.getCommand());
    else
    {
        channelName = message.getParametersIndex(0);
        cTargetNick = message.getParametersIndex(1);
        Channel *channel = server->getChannel(channelName);
        if (!server->getClient(cTargetNick))
            text = makeNoSuchNick(cTargetNick, 0);
        else if(!channel)
            text = makeErrorNoSuchChannel(cNick, channelName);
        else if (!channel->getClient(cNick))
            text = makeErrorNotOnChannel(cNick, channelName);
        else if (!channel->getClient(cTargetNick))
            text = makeErrorNotOnChannel(cTargetNick, channelName);
        else if (!channel->clientHasMode(cNick, 'o'))
            text = makeChanNoPrivsNeeded(cNick, channelName);
        else
        {
            Client *clientTarget = server->getClient(cTargetNick);
            if (message.getSize() == 3)
                text = sendKick(cNick, cTargetNick, message.getLastParameter(), channel);
            else
                text = sendKick(cNick, cTargetNick, "", channel);
            channel->sendToAll(text);
            channel->addMessage(text);
            clientTarget->removeChannel(channelName);
            channel->removeClient(cTargetNick);
            text = "";
        }
    }
    if (text != "")
    {
        std::cout << "Il testo inviato é: " << text << std::endl;
        send(client->getSocketFd(), text.c_str(), text.size(), 0);
    }
    return (0);
}
/********************************/

//*******JOIN**********//

static std::string ft_success_join(Channel *channel, Client *client)
{
    std::string     text;

    std::cout << "Stiamo inviando il join" << std::endl;
    text = ":" + client->getNickname() + " JOIN " + channel->getName() + DEL;
    channel->sendToAll(text, client);
    if (channel->getTopic() != "")
    {
        text.append(makeTopic(channel->getName(), channel->getTopic(), client->getNickname()));
        text.append(makeTopicWhoTime(client->getNickname(), channel));
    }
    std::cout << "ABBIAMO APPESO" << std::endl;
    text.append(makeNamReply(channel, client->getNickname(), 0));
    text.append(makeEndOfNames(channel->getName(), client->getNickname()));
    text.append(channel->getAllMessages());
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
        newChannel = new Channel(channelName, key);
        std::cout << "La password: " << key << std::endl;
        if (newChannel->addClient(client, key, 'o'))
        {
            delete (newChannel);
            return makeErrorBadChannelKey(client->getNickname(), channelName);
        }
        client->addChannel(newChannel);
        server->addChannel(newChannel);
        return (ft_success_join(newChannel, client));
    }
    else
    {
        if (newChannel->isBanned(client->getNickname(), client->getUsername(), client->getRealname()))
            return (makeErrorBannedFromChan(client->getNickname(), newChannel->getName()));
        else if (newChannel->isOnChannel(client->getNickname()))
            return (makeErrorUserOnChannel(client->getNickname(), client->getNickname(), newChannel->getName()));
        else if (newChannel->hasMode('i'))
            return (makeInviteOnlyChan(client->getNickname(), newChannel->getName()));
        else if (newChannel->hasMode('l') && newChannel->getNClient() >= newChannel->getLimit())
            return (makeErrorChannelIsFull(client->getNickname(), newChannel->getName()));
        if (newChannel->addClient(client, key, 0))
            return (makeErrorBadChannelKey(client->getNickname(), newChannel->getName()));
        return (ft_success_join(newChannel, client));
    }
    return ("");
}

int execJoin(Message message, Client *client, Server *server)
{
    std::string     text;
    std::vector<std::string> listChannel;
    std::vector<std::string> listKey;

    std::cout << "LA SIZE E': " << message.getSize() << std::endl;
    if (message.getSize() == 0)
    {
        text = makeErrorNeedMoreParams(client->getNickname(), message.getCommand());
        std::cout << "Il testo inviato é: " << text << std::endl;
        send(client->getSocketFd(), text.c_str(), text.size(), 0);
    }        
    if (message.getSize() >= 1)
        listChannel = ft_split(message.getParametersIndex(0), ',');
    if (message.getSize() >= 2)
        listKey = ft_split(message.getParametersIndex(1), ',');
    if (message.getSize() >= 1)
    {
        std::vector<std::string>::iterator it;
        std::vector<std::string>::iterator keyIt;

        keyIt = listKey.begin();
        for (it = listChannel.begin(); it < listChannel.end(); it++)
        {
            if (server->badChanMask(*it))
                text = makeErrorBadChanMask(*it);
            else if (keyIt < listKey.end())
            {
                text = ft_exec_join(*it, *keyIt, client, server);
                keyIt++;
            }
            else
                text = ft_exec_join(*it, "", client, server);
            std::cout << "Il testo inviato é: " << text << std::endl;
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
        std::vector<Channel *>  channels;
        channels = server->getChannels();
        text += makeRplListStart(client->getNickname());
        for (std::vector<Channel *>::const_iterator it = channels.begin(); it < channels.end(); it++)
        {
            if (!(*it)->hasMode('s') || (*it)->isOnChannel(client->getNickname()))
                text += makeRplList(client->getNickname(), *(*it));
        }
    }
    else
    {
        std::vector<std::string>    parameters;
        parameters = message.getParameters();
        for (std::vector<std::string>::iterator it = parameters.begin(); it < parameters.end(); it++)
        {
            if (server->getChannel(*it))
                text += makeRplList(client->getNickname(), *server->getChannel(*it));
        }
    }
    text += makeRplListEnd(client->getNickname());
    std::cout << "Il testo inviato é: " << text << std::endl;
    return (send(client->getSocketFd(), text.c_str(), text.size(), 0));
}

//***********MODE************//

static void  execSpecialFlag(Channel *channel, std::string param, int flag, char c)
{
    if (c == 'o')
        channel->setOperator(param, flag);
    else if (c == 'b')
        channel->setBanMask(param, flag);
    else if (c == 'k')
        channel->setKey(param, flag);
    else if (c == 'v')
        channel->setModeratorPermission(param, flag);
    else if (c == 'l')
        channel->setLimit(_ft_atoi(param));
    channel->setMode(c, flag);
    std::cout << "Il mode di channel: " << channel->getModes() << std::endl;
}

static void execChannelModeAdd(Channel *channel, Message message, std::string m)
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
        else
        {
            execSpecialFlag(channel, message.getParametersIndex(n), flag, m[u]);
            if (!flag || m[u] == 'o' || m[u] == 'v')
                n++;
        }
    }
}

static std::string  ft_check_mode(Message message, Client client, Channel channel)
{
    int i;
    int n;
    std::string modes;
    std::string aOptions;
    std::string options;

    i = 0;
    n = 2;
    modes = message.getParametersIndex(1);
    options = MODES;
    aOptions = AMODES;
    std::cout << "Siamo dentro mode" << std::endl;
    if (modes[i] == '+' || modes[i] == '-')
        i++;
    for (size_t c = i; c < modes.length(); c++)
    {
        if (options.find(modes[c]) == std::string::npos)
            return (makeErrorUModeUnknownFlag(client.getNickname()));
        if (aOptions.find(modes[c]) != std::string::npos)
        {
            std::cout << "SIAMO nel ciclo FOR" << std::endl;
            if (modes[0] == '+' && message.getParametersIndex(n) == "")
                return (makeErrorNeedMoreParams(client.getNickname(), "MODE"));
            if (modes[c] == 'b' && channel.checkBanMask(message.getParametersIndex(n)))
                return (makeErrorUnKnownError(client.getNickname(), "MODE", "ban mask format is wrong. Command INFO for more details"));
            if (modes[0] == '+' && modes[c] == 'l' && _ft_atoi(message.getParametersIndex(n)) == -1)
                return (makeErrorUnKnownError(client.getNickname(), "MODE", "limit format is wrong"));
            if ((modes[c] == 'o' || modes[c] == 'v') && message.getParametersIndex(n) == "")
                return (makeErrorNeedMoreParams(client.getNickname(), "MODE"));
            if ((modes[c] == 'o' || modes[c] == 'v') && !channel.isOnChannel(message.getParametersIndex(n)))
                return (makeErrorNotOnChannel(message.getParametersIndex(n), channel.getName()));
            n++;
        }
    }
    return ("");
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
        return (makeRplChannelModeis(client->getNickname(), channel));
    text = ft_check_mode(message, *client, *channel);
    if (text != "")
        return (text);
    execChannelModeAdd(channel, message, message.getParametersIndex(1));
    text  = ":" + client->getNickname() + " MODE " + channel->getName() + " " + message.getParametersIndex(1);
    for (int n = 2; n < message.getSize() ;n++)
        text += " " + message.getParametersIndex(n);
    text += DEL;
    channel->addMessage(text);
    channel->sendToAll(text, client);
    return (text);
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
    return (":" + client->getNickname() + " MODE " + client->getNickname() + " " + mode + DEL);
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
    std::cout << "Il testo inviato é: " << text << std::endl;
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
    std::cout << "Il testo inviato é: " << text << std::endl;
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}
//************************************//

int execNames(Message message, Client *client, Server *server)
{
    std::string text;
    std::string channelName;
    Channel     *channel;

    channelName = message.getParametersIndex(0);
    channel = server->getChannel(channelName);
    if (channelName == "")
        text = makeEndOfNames("*", client->getNickname());
    else if (!channel)
        text = makeEndOfNames(channelName, client->getNickname());
    else if (channel->hasMode('s') && !channel->isOnChannel(client->getNickname()))
        text = makeEndOfNames(channelName, client->getNickname());
    else
    {
        text = makeNamReply(channel, client->getNickname(), channel->isOnChannel(client->getNickname()));
        text += makeEndOfNames(channelName, client->getNickname());
    }
    std::cout << "Il testo inviato é: " << text << std::endl;
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
    {
        std::cout << "Il testo inviato é: " << error << std::endl;
        send(client->getSocketFd(), error.c_str(), error.size() + 1, 0);
        return (1);
    }
    return (0);
}

//**********NOTICE*************************//

static int execNoticeChannel(Message message, Client *client, Server *server, std::string target)
{
    Channel *channel;
    std::string text;
    std::vector<std::pair<int, Client *> >::const_iterator it;

    text = "";
    channel = server->getChannel(target);
    if (channel == NULL)
        return (0);
    else if (channel->isBanned(client->getNickname(), client->getUsername(), client->getRealname()))
        return (0);
    else if (channel->hasMode('m') && (!channel->clientHasMode(client->getNickname(), 'v') && !channel->clientHasMode(client->getNickname(), 'v')))
        return (0);
    if (text != "")
        return (send(client->getSocketFd(), text.c_str(), text.size(), 0));
    for (it = channel->getFirstClient(); it < channel->getLastClient(); it++)
    {
        text = ":" + client->getNickname() +  " NOTICE " +  target + " :" + message.getLastParameter() + DEL;
        std::cout << "Il testo inviato é: " << text << std::endl;
        send((*it).second->getSocketFd(), text.c_str(), text.size(), 0);
    }
    return (0);
}

static int execNoticeClient(Message message, Client *client, Server *server, std::string target)
{
    Client      *clientTarget;
    std::string text;

    clientTarget = server->getClient(target);
    if (clientTarget == NULL)
        return (0);
    else if (clientTarget->getAwayStatus())
        return (0);
    else
    {
        text = ":" + client->getNickname() +  " NOTICE " + clientTarget->getNickname() + " :" + message.getLastParameter() + DEL;
        std::cout << "Il testo inviato é: " << text << std::endl;
        send(clientTarget->getSocketFd(), text.c_str(), text.size(), 0);
    }
    return (0);
}

int execNotice(Message message, Client *client, Server *server)
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
            execNoticeChannel(message, client, server, target);
        else
            execNoticeClient(message, client, server, target);
        i++;
    }
    return (0);
}
//*******************************************************************//

int execPart(Message message, Client *client, Server *server)
{
    std::vector<std::string> parameters;
    std::string channelName;
    std::string toSend;
    Channel *channel;

    parameters = message.getParameters();
    for (std::vector<std::string>::iterator it = parameters.begin(); it < parameters.end(); it++)
    {
        channelName = *it;
        channel = server->getChannel(channelName);
        if (channel == NULL)
            toSend = makeErrorNoSuchChannel(client->getNickname(), channelName);
        else if (!channel->getClient(client->getNickname()))
            toSend = makeErrorNotOnChannel(client->getNickname(), channelName);
        if (toSend != "")
            send(client->getSocketFd(), toSend.c_str(), toSend.size(), 0);
        else
        {
            toSend = ":" + client->getNickname() + " PART " + channelName + DEL;
            if (channel->removeClient(client->getNickname()) == -1)
                server->removeChannel(channelName);
            else
            {
                channel->sendToAll(toSend);
                channel->addMessage(toSend);
            }
        }
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
    {
        std::cout << "Il testo inviato é: " << text << std::endl;
        send(client->getSocketFd(), text.c_str(), text.size(), 0);
    }
    return (0);
}

int execPing(Message message, Client *client, Server *server)
{
    std::string     text;

    if (message.getSize() < 1)
        text = makeErrorNeedMoreParams(client->getNickname(), message.getCommand());
    else
        text = "PONG " + server->getServername() + " " + message.getParametersIndex(0) + DEL;
    std::cout << "Il testo inviato é: " << text << std::endl;
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
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
    else if (channel->hasMode('m') && (!channel->clientHasMode(client->getNickname(), 'v') && !channel->clientHasMode(client->getNickname(), 'o')))
        text = makeChanNoPrivsNeeded(client->getNickname(), channel->getName());
    if (text != "")
    {
        std::cout << "Il testo inviato é: " << text << std::endl;
        return (send(client->getSocketFd(), text.c_str(), text.size(), 0));
    }
    text = ":" + client->getNickname() +  " PRIVMSG " +  target + " :" + message.getLastParameter() + DEL;
    for (it = channel->getFirstClient(); it < channel->getLastClient(); it++)
    {
        std::cout << "Il testo inviato é: " << text << std::endl;
        if ((*it).second != client)
            send((*it).second->getSocketFd(), text.c_str(), text.size(), 0);
    }
    channel->addMessage(text);
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
        std::cout << "Il testo inviato é: " << text << std::endl;
        send(client->getSocketFd(), text.c_str(), text.size(), 0);
    }
    else if (clientTarget->getAwayStatus())
    {
        text = makeAway(clientTarget->getUsername(), clientTarget->getNickname(), clientTarget->getAwayMessage());
        std::cout << "Il testo inviato é: " << text << std::endl;
        send(client->getSocketFd(), text.c_str(), text.size(), 0);
    }
    else
    {
        text = ":" + client->getNickname() +  " PRIVMSG " + clientTarget->getNickname() + " :" + message.getLastParameter() + DEL;
        std::cout << "Il testo inviato é: " << text << std::endl;
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
    std::string             text;
    std::vector<Channel *>  channels;
    int                     fd;

    channels = client->getChannels();
    fd = client->getSocketFd();
    text = ":" + client->getNickname() + " QUIT";
    if (message.getIsLastParameter())
        text += " :Quit:" + message.getLastParameter();
    text += DEL;
    for (std::vector<Channel *>::iterator it = channels.begin(); it < channels.end(); it++)
    {
        if ((*it)->removeClient(client->getSocketFd()) == -1)
            server->removeChannel((*it)->getName());
        std::cout << "Il testo inviato é: " << text << std::endl;
        (*it)->sendToAll(text);
    }
    std::cout << "Il testo inviato é: " << text << std::endl;
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    server->removeClient(client->getNickname());
    std::cout << "L'fd é: " << fd << std::endl;
    //close (fd);
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
        else if (message.getIsLastParameter())
        {
            if (!channel->isOnChannel(cNick))
                text = makeErrorNotOnChannel(cNick, channel->getName());
            else if (channel->hasMode('t') && !channel->clientHasMode(cNick, 'o'))
                text = makeChanNoPrivsNeeded(cNick, channel->getName());
            else
            {
                channel->setTopic(message.getParametersIndex(1));
                channel->setTopicTime();
                channel->setTopicSetter(client->getNickname());
                text = ":" + client->getNickname() + " TOPIC " + channel->getName() + " :" + channel->getTopic() + DEL;
                std::cout << "Il testo inviato é: " << text << std::endl;
                std::vector<std::pair<int, Client *> > clients = channel->getClients();
                for (std::vector<std::pair<int, Client *> >::iterator it = clients.begin(); it < clients.end(); it++)
                {
                    std::cout << "Messaggio inviato" << std::endl;
                    if (send((*it).second->getSocketFd(), text.c_str(), text.size(), 0) == -1)
                        return (-1);
                }
                return (0);
            }
        }
        else
        {
            if (channel->getTopic() != "")
                text = makeTopic(channel->getName(), channel->getTopic(), client->getNickname());
            else
                text = makeNoTopic(channel->getName(), client->getNickname());
        }
    }
    std::cout << "Il testo inviato é: " << text << std::endl;
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
    std::cout << "Il testo inviato é: " << text << std::endl;
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);   
}

int execUser(Message message, Client *client)
{
    std::string     username;
    std::string     cNick;
    std::string     error;

    cNick = client->getNickname();
    if (message.getSize() < 4 || !message.getIsLastParameter())
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
        client->setRealname(message.getLastParameter());
        return (1);
    }
    std::cout << "Il testo inviato é: " << error << std::endl;
    send(client->getSocketFd(), error.c_str(), error.size(), 0);
    return (-1);
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
    std::cout << "Il testo inviato é: " << text << std::endl;
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}

//*****************WHO***********************//

static std::string execWhoChannel(Client *client, std::string channelName, Server *server)
{
    std::string text;

    Channel * channel = server->getChannel(channelName);
    if (channel)
    {
        std::vector<std::pair<int, Client *> > list = channel->getClients();
        for (std::vector<std::pair<int, Client *> >::iterator it = list.begin(); it < list.end(); it++)
        {
            if ((!(*it).second->hasMode('i') || server->haveChannelCommon((*it).second, client)))
                text.append(makeWhoReply((*it).second));
        }
    }
    return (text);
}

static std::string execWhoNick(std::string nick, Server *server, int op)
{
    std::string text;

    Client *whoClient = server->getClient(nick);
    if (whoClient && (!whoClient->hasMode('i') || op))
        text.append(makeWhoReply(server->getClient(nick)));
    return (text);
}

int execWho(Message message, Client *client, Server *server)
{
    std::string text;
    Client      *secondNick;

    if (message.getSize() >= 1)
    {
        if (message.getParametersIndex(0)[0] == '&' || message.getParametersIndex(0)[0] == '#')
        {
            if (server->getChannel(message.getParametersIndex(0)))
                text.append(execWhoChannel(client, message.getParametersIndex(0), server));
        }
        else
        {
            secondNick = server->getClient(message.getParametersIndex(0));
            if (secondNick)
                text.append(execWhoNick(message.getParametersIndex(0), server, server->haveChannelCommon(client, secondNick)));
        }
        text.append(makeEndOfWho(client->getNickname(), message.getParametersIndex(0)));
    }
    std::cout << "Il testo inviato é: " << text << std::endl;
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}
//****************************************//

std::string listCommands[22] = {
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
    "PING",
    "PRIVMSG",
    "QUIT",
    "TIME",
    "TOPIC",
    "USER",
    "VERSION",
    "WHO"
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
            return (execInfo(client, server));
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
            return (execPing(message, client, server));
        case 15:
            return (execPrivmsg(message, client, server));
        case 16:
            return (execQuit(message, client, server));
        case 17:
            return (execTime(message, client, server));
        case 18:
            return (execTopic(message, client, server));
        case 19:
            return (execUser(message, client)); 
        case 20:
            return (execVersion(message, client, server));
        case 21:
            return (execWho(message, client, server));
        default:
            return (0);
    }
}
