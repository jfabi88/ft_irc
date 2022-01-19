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
        std::string getMotD() const;
        std::vector<Client*> getClients() const;
        Client  *getClient(int indx) const;
        Client  *getClient(std::string name) const;
        Channel *getChannel(std::string name) const;
        std::vector<std::string> getCapabilities() const;
        std::vector<std::string> getParameter() const;
        std::string returnDate() const;
        
        void    setPort(int newport);
        void    setSocket(int newfd);
        void    setClient(Client *newclient);
        void    setServername(std::string servername);
        void    setVersion(std::string version);
        void    setMotD(std::string motd);
        void    addChannel(Channel *channel);
        void    removeChannel(std::string channelName);

        int     startCommunication(int fdNewClient, char *buffer, Client *client);
        int     receiveCommand(int fdClient, char *buffer);

        int     hasCapability(std::string name) const;
        int     hasCapabilities(std::vector<std::string> prefix) const;
        int     findClient(std::string nickname) const;

        int     verifyPassword(std::string userPassword);
        void    printClients();
        void    removeClient(std::string clientName);

    private:
        int                 _port;
        int                 _fd;
        std::string         _password;
        std::string         _servername;
        std::string         _version;
        std::string         _date;
        std::string         _motd;
        std::vector<Client *> _clients;
        std::vector<Channel *> _channels;
        std::vector<std::string> _capabilities;

        //**PARAMETER**//
        int         _awaylen;
        std::string _casemapping;
        std::string _chanlimit;
        int         _chanlen;
        std::string _chantypes;
        std::string _elist;
        //EXCEPTS
        //EXTBAN
        int         _hostlen;
        //INVEX
        int         _kicklen;
        //MAXLIST
        int         _maxtargets;
        int         _modes;
        std::string _network;
        int         _nicklen;
        //PREFIX
        bool        _safelist;
        //SILENCE
        //STATUSMSG
        //TARGMAX
        int         _topiclen;
        int         _userlen;

        //**START COMMUNICATION**/
        void ft_parse_data(std::vector<std::string> *array, std::string *b, char *buffer);
        std::vector<std::string> ft_take_messages(int fdNewClient, char *buffer);
        int ft_exec_communication_commands(int flag, std::string text, Client *client);

        void ft_memset(char *buffer, int size);
        int  ft_welcome(Client *client);
};

#endif
