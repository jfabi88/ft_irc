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

        int sendReplies(int num, Server serve, Client client);
    private:
        const static int   size = 3;
        std::string makeWelcome(Server server, Client client);
        std::string makeYourHost(Server server, Client client);
        std::string makeCreated(Server server, Client client);
        typedef std::string (RepliesCreator::*fct_point)(Server, Client);
        static fct_point   array[];
        static int Repliess[];
};

#endif
