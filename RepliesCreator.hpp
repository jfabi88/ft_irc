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

#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class RepliesCreator
{
    public:
        RepliesCreator();
        ~RepliesCreator();

        std::string makeWelcome(Client client);
        std::string makeYourHost(Server server, Client client);
        std::string makeCreated(Server server, Client client);
        std::string makeErrorNeedMoreParams(Client client, std::string command);
        std::string makeErrorAlreadyRegistered(Client client);
        std::string makePasswdMisMatch(Client client);
    private:
};

#endif
