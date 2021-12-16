/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RepliesCreator.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 15:08:40 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/11 15:08:42 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_CREATOR
#define REPLIES_CREATOR

#include <iostream>

class RepliesCreator
{
    public:
        RepliesCreator();
        ~RepliesCreator();

        std::string makeWelcome(std::string CNickname);
        std::string makeYourHost(std::string Servname, std::string SVersion, std::string CNickname);
        std::string makeCreated(std::string SDate, std::string CNick);

        std::string makeErrorNeedMoreParams(std::string CNick, std::string command);
        std::string makeErrorAlreadyRegistered(std::string CNick);
        std::string makeErrorNoNickNameGiven(std::string CNick);
        std::string makeErrorErroneusNickName(std::string CNick, std::string nick);
        std::string makeErrorNickNameInUse(std::string CNick, std::string nick);
        std::string makePasswdMisMatch(std::string CNick);
    private:
};

#endif
