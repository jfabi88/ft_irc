#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include "Message.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

class ICommand
{
    public:
        virtual ~ICommand();

        virtual void exec() const = 0;
};

#endif