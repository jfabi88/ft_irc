#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "ICommand.hpp"
#include "Server.hpp"
#include "Client.hpp"

class Privmsg : public ICommand
{
    public:
        Privmsg(Message message, Server server, Client client);
        ~Privmsg();

        void    exec() const;
    private:
        Message message;
        Server  server;
        Client  client;
};

#endif