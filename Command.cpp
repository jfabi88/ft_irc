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

    subcommand = message.getParametersIndex(0);
    std::cout << subcommand << std::endl;
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

int execPass(Message message, Client *client)
{
    RepliesCreator  reply;
    std::string     cNick;

    cNick = client->getNickname();
    if (client->getRegistered())
        reply.makeErrorAlreadyRegistered(cNick);
    else if (message.getParametersIndex(0) == "")
        reply.makeErrorNeedMoreParams(cNick, "PASS");
    else
        client->setPassword(message.getParametersIndex(0));
    return (0);
}

int execRPing(Message message, Client *client)
{
    RepliesCreator  reply;
    std::string     text;

    if (message.getParametersIndex(0) == "")
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

    text = "PING SiamoTuttiSimpatici";
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}

int execPrivmsg(Message message, Client *client, Server *server)
{
    std::string target;
    Client      *clientTarget;
    std::string text;

    target = message.getParametersIndex(0);
    clientTarget = server->getClient(target);
    if (clientTarget == NULL)
        return (0);
    text = ":" + client->getNickname() + " " + text;
    //if (clientTarget->getAway())
        //sent reply;
    //else
        send(clientTarget->getSocketFd(), text.c_str(), text.size(), 0);
    delete clientTarget;
    return (0);
}

int execUser(Message message, Client *client)
{
    RepliesCreator  reply;
    std::string     username;
    std::string     cNick;
    std::string     error;

    cNick = client->getNickname();
    if (message.getParametersIndex(0) == "" || message.getParametersIndex(3) == "")
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
