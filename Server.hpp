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
#include "Client.hpp"
#include <time.h>
#include <string>

class Server
{
    public:
        Server();
        Server(int port, int fd);

        int     getPort() const;
        int     getSocket() const;
        std::string getServername() const;
        std::string getVersion() const;
        std::string getDate() const;
        Client  getClient(int indx) const;
        Client  *getClient(std::string name) const;
        std::vector<std::string> getCapabilities() const;

        void    setPort(int newport);
        void    setSocket(int newfd);
        void    setClient(Client newclient);
        void    setServername(std::string servername);
        void    setVersion(std::string version);

        int     startCommunication(int fdNewClient);
        int     isCapability(std::string name) const;
    private:
        int                 port;
        int                 fd;
        std::string         servername;
        std::string         version;
        std::string         date;
        std::string         ft_set_date();
        std::vector<Client> clients;
        std::vector<std::string> capabilities;
};

#endif
