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

RepliesCreator::RepliesCreator()
{
    std::cout << "RepliesCreator created" << std::endl;
}

RepliesCreator::~RepliesCreator()
{
    std::cout << "RepliesCreator deleted" << std::endl;
}

/**PUBBLIC-FUNCTIONS**/

std::string RepliesCreator::makeWelcome(Client client)
{
    std::string text;
 
    text =  client.getNickname() + " :Welcome to the networkname Network, " \
            + client.getNickname() + "\r\n";
    return (text);
}

std::string RepliesCreator::makeYourHost(Server server, Client client)
{
    std::string text;

    text =  client.getNickname() + " :Your host is " + server.getServername() + \
            ", running version " + server.getVersion();
    return (text);
}

std::string RepliesCreator::makeCreated(Server server, Client client)
{
    std::string text;

    text = client.getNickname() + " :This server was created " + server.getDate();
    return (text);
}

std::string RepliesCreator::makeErrorNeedMoreParams(Client client, std::string command)
{
    std::string text;

    text = client.getNickname() + " " + command + " :Not enough parameters";
    return (text);
}

std::string RepliesCreator::makeErrorAlreadyRegistered(Client client)
{
    std::string text;

    text = client.getNickname() + " :You may not reregister";
    return (text);
}

std::string RepliesCreator::makePasswdMisMatch(Client client)
{
    std::string text;

    text = client.getNickname() + " :Password incorrect";
    return (text);
}
