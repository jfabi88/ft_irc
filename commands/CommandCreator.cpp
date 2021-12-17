#include "CommandCreator.hpp"

typedef ICommand *(CommandCreator::*fct_point)();
fct_point CommandCreator::array[] = {
    &CommandCreator::makePrivmsg,
    &CommandCreator::makeCap,
    &CommandCreator::makeNick,
    &CommandCreator::makePass,
    &CommandCreator::makePing,
    &CommandCreator::makeUser
};

std::string CommandCreator::commands[] = {
    "PRIVMSG",
    "CAP",
    "NICK",
    "PASS",
    "PING",
    "USER"
};

CommandCreator::CommandCreator()
{
    std::cout << "CommandCreator created" << std::endl;
}

CommandCreator::~CommandCreator()
{
    std::cout << "CommandCreator deleted" << std::endl;
}

ICommand *CommandCreator::makeCommand(Message &message)
{
    int i = 0;
    ICommand    *newCommand;

    newCommand = NULL;
    while (this->commands[i].compare(message.getCommand()) && i < this->size)
        i++;
    if (i < this->size)
        newCommand = (this->*array[i])();
    return (newCommand);
}

ICommand *CommandCreator::makePrivmsg()
{
    ICommand *ret = new Privmsg();

    return (ret);
}

ICommand *CommandCreator::makeCap()
{
    ICommand *ret = new Cap();

    return (ret);
}

ICommand *CommandCreator::makeNick()
{
    ICommand *ret = new Nick();

    return (ret);
}

ICommand *CommandCreator::makePass()
{
    ICommand *ret = new Pass();

    return (ret);
}

ICommand *CommandCreator::makePing()
{
    ICommand *ret = new RPing();

    return (ret);
}

ICommand *CommandCreator::makeUser()
{
    ICommand *ret = new User();

    return (ret);
}