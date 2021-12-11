#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include "Message.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>

class ICommand
{
    public:
        virtual ~ICommand() {};

        virtual void exec() const = 0;
    protected:
        ICommand(Message newmessage, Server newserver, Client newclient) {
            this->message = newmessage;
            this->server = newserver;
            this->client = newclient;
        };
        Message     message;
        Server      server;
        Client      client;
};

#endif