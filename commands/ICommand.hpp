#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include "../Message.hpp"
#include "../Client.hpp"
#include "../Server.hpp"

#include <iostream>

class ICommand
{
    public:
        virtual ~ICommand() {};

        virtual std::string getCommand() {
            return (this->command);
        };
        virtual void exec(Message newmessage, Client newclient, Server server) = 0;
    protected:
        std::string command;
};

#endif