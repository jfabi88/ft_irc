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
#include "Client.hpp"

std::string makeWelcome(std::string CNickname, std::string CUsername, std::string SName)
{
    std::string text;
 
    text =  "001 "+ CNickname + " :Welcome to the" + SName +  \
            "Network, " + CUsername + DEL;
    return (text);
}

std::string makeYourHost(std::string Servname, std::string SVersion, std::string CNickname)
{
    std::string text;

    text =  "002 " + CNickname + " :Your host is " + Servname + \
            ", running version " + SVersion + DEL;
    return (text);
}

std::string makeCreated(std::string SDate, std::string CNick)
{
    std::string text;

    text = "003 " + CNick + " :This server was created " + SDate + DEL;
    return (text);
}

std::string makeAway(std::string Client, std::string CNick, std::string message)
{
    std::string text;

    text = "301 " + Client + " " + CNick + " :" + message + DEL;
    return (text);
}

std::string makeUnAway(std::string Client)
{
    std::string text;

    text = "305 " + Client + " :You are no longer maarked as being away" + DEL;
    return (text);
}

std::string makeNowAway(std::string Client)
{
    std::string text;

    text = "306 " + Client + " :You have been marked ad being away" + DEL;
    return (text);
}

std::string makeTopic(std::string channelName, std::string topic, std::string CNick)
{
    std::string text;

    text = "332 " + CNick + " " + channelName + " :" + topic + DEL; 
    return (text);
}

std::string makeInviting(std::string CNick, std::string CNickTarget, std::string channel)
{
    std::string text;

    text = "341 " + CNick + " " + CNickTarget + " " + channel + DEL;
    return (text);
}

std::string makeNamReply(Channel channel, std::string CNick)
{
    std::string text;

    text = "353 " + CNick + " " + channel.getSymbol() + " " + channel.getName() + " :";
    std::vector<t_PChannel>::const_iterator it;
    for (it = channel.getFirstClient(); it < channel.getLastClient() ;it++)
    {
        if (((*it).prefix) != 0)
        {
            text += (*it).prefix;
            text.append(" ");
        }
        text += (*it).client->getNickname() + " ";
    }
    text += DEL;
    return (text);
}

std::string makeEndOfNames(std::string channelName, std::string CNick)
{
    std::string text;

    text = "366 " + CNick + " " + channelName + " :End of /NAMES list" + DEL;
    return (text);
}

std::string makeNoSuchNick(std::string CNick, int flag)
{
    std::string text;

    if (flag)
        text = "401 " + CNick + " :No such channel" + DEL;
    else
        text = "401 " + CNick + " :No such nick" + DEL;
    return (text);
}

std::string makeErrorNoSuchChannel(std::string CNick, std::string channelName)
{
    std::string text;

    text = "403 " + CNick + " " + channelName  + " :No such channel" + DEL;
    return (text);
}

std::string makeCannotSendToChan(std::string CNick, std::string channelName)
{
    std::string text;

    text = "404 " + CNick + " " + channelName  + " :Cannot send to channel" + DEL;
    return (text);
}

std::string makeTooManyChannels(std::string CNick, std::string ChannelName)
{
    std::string text;

    text = "405 " + CNick + ChannelName + " :You have joined too many channels" + DEL;
    return (text);
}

std::string makeErrorNoNickNameGiven(std::string CNick)
{
    std::string text;

    text = "431 " + CNick + " :No nickname given" + DEL;
    return (text);
}

std::string makeErrorErroneusNickName(std::string CNick, std::string nick)
{
    std::string text;

    text = "432 " + CNick + " " + nick + " :Erroneus" + DEL;
    return (text);
}

std::string makeErrorNickNameInUse(std::string CNick, std::string nick)
{
    std::string text;

    text = "433 " + CNick + " " + nick + " :Nickname is already in use" + DEL;
    return (text);
}

std::string makeErrorNotOnChannel(std::string CNick, std::string channelName)
{
    std::string text;

    text = "441 " + CNick + " " + channelName  + " :You're not on that channel" + DEL;
    return (text);
}

std::string makeErrorUserOnChannel(std::string CNick, std::string CNickTarget, std::string channelName)
{
    std::string text;

    text = "443 " + CNick + " " + CNickTarget + " " + channelName  + " :is already on channel" + DEL;
    return (text);
}

std::string makeErrorNeedMoreParams(std::string CNick, std::string command)
{
    std::string text;

    text = "461 " + CNick + " " + command + " :Not enough parameters" + DEL;
    return (text);
}

std::string makeErrorAlreadyRegistered(std::string CNick)
{
    std::string text;

    text = "462 " + CNick + " :You may not reregister" + DEL;
    return (text);
}

std::string makePasswdMisMatch(std::string CNick)
{
    std::string text;

    text = "464 " + CNick + " :Password incorrect" + DEL;
    return (text);
}

std::string makeErrorChannelIsFull(std::string CNick, std::string channelName)
{
    std::string text;

    text = "471 " + CNick + " " + channelName + " :Cannot join channel (+l)" + DEL;
    return (text);   
}

std::string makeInviteOnlyChan(std::string CNick, std::string channelName)
{
    std::string text;

    text = "473 " + CNick + " " + channelName + " :Cannot join channel (+i)" + DEL;
    return (text);
}

std::string makeErrorBannedFromChan(std::string CNick, std::string channelName)
{
    std::string text;

    text = "474 " + CNick + " " + channelName + " :Cannot join channel (+b)" + DEL;
    return (text);
}

std::string makeErrorBadChannelKey(std::string CNick, std::string channelName)
{
    std::string text;

    text = "475 " + CNick + " " + channelName + " :Cannot join channel (+k)" + DEL;
    return (text);
}

std::string makeErrorBadChanMask(std::string channel)
{
    std::string text;

    text = "476 " + channel + " :Bad Channel Mask" + DEL;
    return (text);
}

std::string makeChanNoPrivsNeeded(std::string CNick, std::string channel)
{
    std::string text;

    text = "482 " + CNick + " " + channel + " :You're not channel operator" + DEL;
    return (text);
}
