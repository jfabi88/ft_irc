/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 18:35:36 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/09 18:36:07 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "Message.hpp"
#include "RepliesCreator.hpp"

class Server
{
    public:
        Server();
        Server(int port, int fd, std::string password);

        int     getPort() const;
        int     getSocket() const;
        std::string getServername() const;
        std::string getVersion() const;
        std::string getDate() const;
        std::vector<Client*> getClients() const;
        Client  *getClient(int indx) const;
        Client  *getClient(std::string name) const;
        Channel *getChannel(std::string name) const;
        std::vector<std::string> getCapabilities() const;

        void    setPort(int newport);
        void    setSocket(int newfd);
        void    setClient(Client *newclient);
        void    setServername(std::string servername);
        void    setVersion(std::string version);
        void    addChannel(Channel *channel);

        int     startCommunication(int fdNewClient, char *buffer);
        int     receiveCommand(int fdClient, char *buffer);

        int     hasCapability(std::string name) const;
        int     hasCapabilities(std::vector<std::string> prefix) const;
        int     findClient(std::string nickname) const;

        int     verifyPassword(std::string userPassword);
        void    printClients();
    private:
        int                 port;
        int                 fd;
        std::string         password;
        std::string         servername;
        std::string         version;
        std::string         date;
        std::vector<Client *> clients;
        std::vector<Channel *> channels;
        std::vector<std::string> capabilities;

        std::string         ft_set_date();

        //**START COMMUNICATION**/
        void ft_parse_data(std::vector<std::string> *array, std::string *b, char *buffer);
        std::vector<std::string> ft_take_messages(int fdNewClient, char *buffer);
        int ft_exec_communication_commands(int flag, std::string text, Client *client);

        void ft_memset(char *buffer, int size);
        int  ft_welcome(Client *client);
};

#endif
