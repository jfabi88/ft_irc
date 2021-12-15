#include "CommandCreator.hpp"

typedef ICommand *(CommandCreator::*fct_point)(Message, Server, Client);
fct_point CommandCreator::array[] = {
    &CommandCreator::makePrivmsg
};

std::string CommandCreator::commands[] = {
    "PRIVMSG",
    "CAP",
    "NICK",
    "PASS",
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

ICommand *CommandCreator::makeCommand(Message message, Server server, Client client)
{
    int i = 0;
    ICommand    *newCommand;

    newCommand = NULL;
    while (!(this->commands[i].compare(message.getCommand())) && i < this->size)
        i++;
    if (i < this->size)
        newCommand = (this->*array[i])(message, server, client);
    return (newCommand);
}

ICommand *CommandCreator::makePrivmsg(Message message, Server server, Client client)
{
    ICommand *ret = new Privmsg(message, server, client);

    return (ret);
}

ICommand *CommandCreator::makeCap(Message message, Server server, Client client)
{
    ICommand *ret = new Cap(message, server, client);

    return (ret);
}

ICommand *CommandCreator::makeNick(Message message, Server server, Client client)
{
    ICommand *ret = new Nick(message, server, client);

    return (ret);
}

ICommand *CommandCreator::makePass(Message message, Server server, Client client)
{
    ICommand *ret = new Pass(message, server, client);

    return (ret);
}

ICommand *CommandCreator::makeUser(Message message, Server server, Client client)
{
    ICommand *ret = new User(message, server, client);

    return (ret);
}