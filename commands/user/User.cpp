/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 19:30:34 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/14 19:30:40 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User()
{
    this->command = "USER";
    std::cout << "User created" << std::endl;
}

User::~User()
{
    std::cout << "User deleted" << std::endl;
}

int User::exec(Message message, Client *client, Server server)
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
