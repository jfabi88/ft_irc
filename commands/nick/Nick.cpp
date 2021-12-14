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

Nick::Nick(Message newmessage, Server newserver, Client newclient) : ICommand(newmessage, newserver, newclient)
{
    std::cout << "Nick created" << std::endl;
}

Nick::~Nick()
{
    std::cout << "Nick deleted" << std::endl;
}

void Nick::exec()
{
    RepliesCreator  reply;
    std::string     nick;
    std::string     banCharacters = "?";
    int             i;

    nick = this->message.getLastParameter();
    if (nick == "")
        reply.makeErrorNoNickNameGiven(this->client);
    else if (this->server.findClient(nick) != -1)
        reply.makeErrorNickNameInUse(this->client, nick);
    for (i = 0; i < nick.size(); i++)
    {
        if (banCharacters.find(nick[i]))
            reply.makeErrorErroneusNickName(this->client, nick);
    }
    if (i == nick.size())
        this->client.setNickname(nick);
}
