#ifndef COMMAND_CREATOR
#define COMMAND_CREATOR

#include "ICommand.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Privmsg.hpp"

class CommandCreator
{
    public:
        CommandCreator();
        ~CommandCreator();

        ICommand    *makeCommand(Message message, Server serve, Client client);
    private:
        const static int   size = 1;
        ICommand    *makePrivmsg(Message message, Server serve, Client client);

        typedef ICommand *(CommandCreator::*fct_point)(Message, Server, Client);
        fct_point   array[size] = {
            &CommandCreator::makePrivmsg
        };
        std::string commands[size] = {
            "PRIVMSG"
        };
};

#endif