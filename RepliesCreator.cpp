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

std::string RepliesCreator::makeWelcome(std::string CNickname)
{
    std::string text;
 
    text =  CNickname + " :Welcome to the networkname Network, " \
            + CNickname + "\\r\n";
    return (text);
}

std::string RepliesCreator::makeYourHost(std::string Servname, std::string SVersion, std::string CNickname)
{
    std::string text;

    text =  CNickname + " :Your host is " + Servname + \
            ", running version " + CNickname + "\\r\n";
    return (text);
}

std::string RepliesCreator::makeCreated(std::string SDate, std::string CNick)
{
    std::string text;

    text = CNick + " :This server was created " + SDate + "\\r\n";
    return (text);
}

std::string RepliesCreator::makeErrorNeedMoreParams(std::string CNick, std::string command)
{
    std::string text;

    text = CNick + " " + command + " :Not enough parameters" + "\\r\n";
    return (text);
}

std::string RepliesCreator::makeErrorAlreadyRegistered(std::string CNick)
{
    std::string text;

    text = CNick + " :You may not reregister" + "\\r\n";
    return (text);
}

std::string RepliesCreator::makePasswdMisMatch(std::string CNick)
{
    std::string text;

    text = CNick + " :Password incorrect" + "\\r\n";
    return (text);
}

std::string RepliesCreator::makeErrorNoNickNameGiven(std::string CNick)
{
    std::string text;

    text = CNick + " :No nickname given" + "\\r\n";
    return (text);
}

std::string RepliesCreator::makeErrorErroneusNickName(std::string CNick, std::string nick)
{
    std::string text;

    text = CNick + " " + nick + " :Erroneus" + "\\r\n";
    return (text);
}

std::string RepliesCreator::makeErrorNickNameInUse(std::string CNick, std::string nick)
{
    std::string text;

    text = CNick + " " + nick + " :Nickname is already in use" + "\\r\n";
    return (text);
}
