/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RepliesCreator.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 15:08:40 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/11 15:08:42 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_CREATOR
#define REPLIES_CREATOR

#include <iostream>
#include <vector>
#include "Channel.hpp"

#define DEL "\r\n"
#define DELSIZE 2

class RepliesCreator
{
    public:
        RepliesCreator();
        ~RepliesCreator();

        std::string makeWelcome(std::string CNickname, std::string CUsername, std::string SName);
        std::string makeYourHost(std::string Servname, std::string SVersion, std::string CNickname);
        std::string makeCreated(std::string SDate, std::string CNick);

        std::string makeErrorNeedMoreParams(std::string CNick, std::string command);
        std::string makeErrorAlreadyRegistered(std::string CNick);
        std::string makeErrorNoNickNameGiven(std::string CNick);
        std::string makeErrorErroneusNickName(std::string CNick, std::string nick);
        std::string makeErrorNickNameInUse(std::string CNick, std::string nick);
        std::string makeErrorBadChannelKey(std::string CNick, std::string channelName);
        std::string makeErrorBadChanMask(std::string channel);
        std::string makePasswdMisMatch(std::string CNick);
        std::string makeErrorBannedFromChan(std::string CNick, std::string channelName);
        std::string makeInviteOnlyChan(std::string CNick, std::string channelName);
        std::string makeBadChannelKey(std::string CNick, std::string channelName);
        std::string makeErrorChannelIsFull(std::string CNick, std::string channelName);
        std::string makeErrorNoSuchChannel(std::string CNick, std::string channelName);
        std::string makeErrorNotOnChannel(std::string CNick, std::string channelName);

        std::string makeAway(std::string Client, std::string CNick, std::string message);
        std::string makeUnAway(std::string Client);
        std::string makeNowAway(std::string Client);
        std::string makeNoSuchNick(std::string CNick, int flag);
        std::string makeTooManyChannels(std::string CNick, std::string ChannelName);

        std::string makeTopic(std::string channelName, std::string topic, std::string CNick);
        std::string makeNamReply(Channel channel, std::string CNick);
        std::string makeEndOfNames(std::string channelName, std::string CNick);
    private:
};

#endif
