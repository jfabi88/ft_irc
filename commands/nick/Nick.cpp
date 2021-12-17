/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 18:04:25 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/14 18:04:26 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nick.hpp"

Nick::Nick()
{
    this->command = "NICK";
    std::cout << "Nick created" << std::endl;
}

Nick::~Nick()
{
    std::cout << "Nick deleted" << std::endl;
}

int Nick::exec(Message message, Client *client, Server server)
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
    else if (server.findClient(nick) != -1)
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
    