/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RepliesCreator.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smenna <smenna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 15:08:40 by jfabi             #+#    #+#             */
/*   Updated: 2022/01/18 15:49:53 by smenna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_CREATOR
# define REPLIES_CREATOR

#include <iostream>
#include <vector>
#include <utils.hpp>
#include "Channel.hpp"

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
std::string makeCannotSendToChan(std::string CNick, std::string channelName);
std::string makeAway(std::string Client, std::string CNick, std::string message);
std::string makeUnAway(std::string Client);
std::string makeNowAway(std::string Client);
std::string makeNoSuchNick(std::string CNick, int flag);
std::string makeTooManyChannels(std::string CNick, std::string ChannelName);

std::string makeTopic(std::string channelName, std::string topic, std::string CNick);
std::string makeNamReply(Channel channel, std::string CNick);
std::string makeEndOfNames(std::string channelName, std::string CNick);
std::string makeChanNoPrivsNeeded(std::string CNick, std::string channel);
std::string makeErrorUserOnChannel(std::string CNick, std::string CNickTarget, std::string channelName);
std::string makeInviting(std::string CNick, std::string CNickTarget, std::string channel);

#endif
