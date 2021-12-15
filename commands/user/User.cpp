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

User::User(Message newmessage, Server newserver, Client newclient) : ICommand(newmessage, newserver, newclient)
{
    std::cout << "User created" << std::endl;
}

User::~User()
{
    std::cout << "User deleted" << std::endl;
}

void User::exec()
{
    RepliesCreator  reply;
    std::string     username;

    if (this->message.getParametersIndex(0) == "")
        reply.makeErrorNeedMoreParams(this->client, "USER");
    else if (this->client.getUsername() != "")
        reply.makeErrorAlreadyRegistered(this->client);
    username = this->message.getParametersIndex(0);
    if (username.size() > USERLEN)
        this->client.setUsername(username.substr(0, USERLEN));
    else
        this->client.setUsername(username);
    this->client.setRealname(message.getParametersIndex(3));   
}
