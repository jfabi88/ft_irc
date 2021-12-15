#ifndef COMMAND_CREATOR
#define COMMAND_CREATOR

#include "ICommand.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"

#include "commands/cap/Cap.hpp"
#include "commands/nick/Nick.hpp"
#include "commands/pass/Pass.hpp"
#include "commands/privmsg/Privmsg.hpp"
#include "commands/user/User.hpp"

class CommandCreator
{
    public:
        CommandCreator();
        ~CommandCreator();

        ICommand    *makeCommand(Message message, Server serve, Client client);
    private:
        const static int size = 5;
        typedef ICommand *(CommandCreator::*fct_point)(Message, Server, Client);
        static fct_point   array[];
        static std::string commands[];

        ICommand    *makePrivmsg(Message message, Server serve, Client client);
        ICommand    *makeCap(Message message, Server serve, Client client);
        ICommand    *makeNick(Message message, Server serve, Client client);
        ICommand    *makePass(Message message, Server serve, Client client);
        ICommand    *makeUser(Message message, Server serve, Client client);
};

#endif