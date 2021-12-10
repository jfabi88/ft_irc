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

class Server
{
    public:
        Server();
        Server(int port, int fd);

        int     getPort() const;
        int     getSocket() const;
        Client  getClient(int indx) const;
        Client  *getClient(std::string name) const;

        void    setPort(int newport);
        void    setSocket(int newfd);
        void    setClient(Client newclient);
    private:
        int port;
        int fd;
        std::vector<Client> clients;
};

#endif
