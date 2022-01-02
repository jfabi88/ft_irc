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

std::string RepliesCreator::makeWelcome(std::string CNickname, std::string CUsername, std::string SName)
{
    std::string text;
 
    text =  CNickname + " :Welcome to the" + SName +  \
            "Network, " + CUsername + DEL;
    return (text);
}

std::string RepliesCreator::makeYourHost(std::string Servname, std::string SVersion, std::string CNickname)
{
    std::string text;

    text =  CNickname + " :Your host is " + Servname + \
            ", running version " + SVersion + DEL;
    return (text);
}

std::string RepliesCreator::makeCreated(std::string SDate, std::string CNick)
{
    std::string text;

    text = CNick + " :This server was created " + SDate + DEL;
    return (text);
}

std::string RepliesCreator::makeErrorNeedMoreParams(std::string CNick, std::string command)
{
    std::string text;

    text = CNick + " " + command + " :Not enough parameters" + DEL;
    return (text);
}

std::string RepliesCreator::makeErrorAlreadyRegistered(std::string CNick)
{
    std::string text;

    text = CNick + " :You may not reregister" + DEL;
    return (text);
}

std::string RepliesCreator::makePasswdMisMatch(std::string CNick)
{
    std::string text;

    text = CNick + " :Password incorrect" + DEL;
    return (text);
}

std::string RepliesCreator::makeErrorNoNickNameGiven(std::string CNick)
{
    std::string text;

    text = CNick + " :No nickname given" + DEL;
    return (text);
}

std::string RepliesCreator::makeErrorErroneusNickName(std::string CNick, std::string nick)
{
    std::string text;

    text = CNick + " " + nick + " :Erroneus" + DEL;
    return (text);
}

std::string RepliesCreator::makeErrorNickNameInUse(std::string CNick, std::string nick)
{
    std::string text;

    text = CNick + " " + nick + " :Nickname is already in use" + DEL;
    return (text);
}

std::string RepliesCreator::makeErrorBadChanMask(std::string channel)
{
    std::string text;

    text = channel + " :Bad Channel Mask" + DEL;
    return (text);
}

std::string RepliesCreator::makeErrorBannedFromChan(std::string CNick, std::string channelName)
{
    std::string text;

    text = CNick + " " + channelName + " :Cannot join channel (+b)" + DEL;
    return (text);
}

std::string RepliesCreator::makeInviteOnlyChan(std::string CNick, std::string channelName)
{
    std::string text;

    text = CNick + " " + channelName + " :Cannot join channel (+i)" + DEL;
    return (text);
}

std::string RepliesCreator::makeBadChannelKey(std::string CNick, std::string channelName)
{
    std::string text;

    text = CNick + " " + channelName + " :Cannot join channel (+k)" + DEL;
    return (text);
}

std::string RepliesCreator::makeErrorChannelIsFull(std::string CNick, std::string channelName)
{
    std::string text;

    text = CNick + " " + channelName + " :Cannot join channel (+l)" + DEL;
    return (text);   
}

std::string RepliesCreator::makeAway(std::string Client, std::string CNick, std::string message)
{
    std::string text;

    text = Client + " " + CNick + " :" + message + DEL;
    return (text);
}

std::string RepliesCreator::makeUnAway(std::string Client)
{
    std::string text;

    text = Client + " :You are no longer maarked as being away" + DEL;
    return (text);
}

std::string RepliesCreator::makeNowAway(std::string Client)
{
    std::string text;

    text = Client + " :You have been marked ad being away" + DEL;
    return (text);
}

std::string RepliesCreator::makeNoSuchNick(std::string CNick, int flag)
{
    std::string text;

    if (flag)
        text = CNick + " :No such channel" + DEL;
    else
        text = CNick + " :No such nick" + DEL;
    return (text);
}

std::string RepliesCreator::makeTooManyChannels(std::string CNick, std::string ChannelName)
{
    std::string text;

    text = CNick + " " + ChannelName + " :You have joined too many channels" + DEL;
    return (text);
}

std::string makeTopic(std::string channelName, std::string topic, std::string CNick)
{
    std::string text;

    text = CNick + " " + channelName + " :" + topic + DEL; 
    return (text);
}

std::string RepliesCreator::makeNamReply(Channel channel, std::string CNick)
{
    std::string text;

    text = CNick + " " + channel.getSymbol() + " " + channel.getName() + " :";
    std::vector<t_PChannel>::const_iterator it;
    for (it = channel.getFirstClient(); it < channel.getLastClient() ;it++)
    {
        if (((*it).prefix) != 0)
            text += (*it).prefix + " ";
        text += (*it).client->getNickname() + " ";
    }
    text += DEL;
    return (text);
}

std::string RepliesCreator::makeEndOfNames(std::string channelName, std::string CNick)
{
    std::string text;

    text = CNick + " " + channelName + " :End of /NAMES list";
    return (text);
}
