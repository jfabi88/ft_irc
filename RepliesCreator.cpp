/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RepliesCreator.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 15:08:33 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/11 15:08:34 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RepliesCreator.hpp"

typedef std::string (RepliesCreator::*fct_point)(Server, Client);
fct_point RepliesCreator::array[] = {
    &RepliesCreator::makeWelcome,
    &RepliesCreator::makeYourHost,
    &RepliesCreator::makeCreated
};

int RepliesCreator::Repliess[] = {
    1,
    2,
    3
};

RepliesCreator::RepliesCreator()
{
    std::cout << "RepliesCreator created" << std::endl;
}

RepliesCreator::~RepliesCreator()
{
    std::cout << "RepliesCreator deleted" << std::endl;
}

int RepliesCreator::sendReplies(int num, Server server, Client client)
{
    std::string text;
    int ret;
    int i;

    ret = -1;
    i = 0;
    while (i < this->size && this->Repliess[i] != num)
        i++;
    if (i < this->size)
    {
        text = (this->*array[i])(server, client);
        ret = send(client.getSocketFd(), text.c_str(), text.size(), 0);
    }
    return (ret);
}

/**PRIVATE-FUNCTIONS**/

std::string RepliesCreator::makeWelcome(Server server, Client client)
{
    std::string text;
 
    text = ":default 001 " + client.getNickname() + \
            " :Welcome to the networkname Network, " + client.getNickname() + \
            "\r\n";
    return (text);
}

std::string RepliesCreator::makeYourHost(Server server, Client client)
{
    std::string text;

    text = ":default 002" + client.getNickname() + \
            " :Your host is " + server.getServername() + \
            ", running version " + server.getVersion();
    return (text);
}

std::string RepliesCreator::makeCreated(Server server, Client client)
{
    std::string text;

    text = ":default 003" + client.getNickname() + \
            " :This server was created " + server.getDate();
    return (text);
}
