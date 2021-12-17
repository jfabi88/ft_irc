/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 18:30:52 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/17 18:30:54 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPing.hpp"

RPing::RPing()
{
    std::cout << "RPing created" << std::endl;
}

RPing::~RPing()
{
    std::cout << "RPing created" << std::endl;
}

int RPing::exec(Message message, Client *client, Server server)
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
