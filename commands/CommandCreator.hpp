#ifndef COMMAND_CREATOR
#define COMMAND_CREATOR

#include "ICommand.hpp"

#include "cap/Cap.hpp"
#include "nick/Nick.hpp"
#include "pass/Pass.hpp"
#include "privmsg/Privmsg.hpp"
#include "user/User.hpp"

class CommandCreator
{
    public:
        CommandCreator();
        ~CommandCreator();

        ICommand    *makeCommand(Message &message);
    private:
        const static int size = 5;
        typedef ICommand *(CommandCreator::*fct_point)();
        static fct_point   array[];
        static std::string commands[];

        ICommand    *makePrivmsg();
        ICommand    *makeCap();
        ICommand    *makeNick();
        ICommand    *makePass();
        ICommand    *makeUser();
};

#endif
