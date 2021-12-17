/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 13:46:08 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/16 13:46:41 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC
#define FT_IRC

#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "commands/cap/Cap.hpp"
#include "commands/nick/Nick.hpp"
#include "commands/pass/Pass.hpp"
#include "commands/privmsg/Privmsg.hpp"
#include "commands/user/User.hpp"

#include "commands/CommandCreator.hpp"
#include "commands/ICommand.hpp"

#include "Client.hpp"
#include "Message.hpp"
#include "RepliesCreator.hpp"
#include "Server.hpp"

#define DEL "\n"
#define DELSIZE 1

class Irc
{
    public:
        Irc();
        ~Irc();

        int     startCommunication(int fdNewClient);
        void    printClients();
    private:
        Server server;

        void ft_parse_data(std::vector<std::string> *array, std::string *b, char *buffer);
        std::vector<std::string> ft_take_messages(int fdNewClient);
        int ft_exec_communication_commands(int flag, std::string text, Client *client);
        void ft_memset(char *buffer, int size);
        int  ft_welcome(Client *client);
};

#endif
