#include "CommandCreator.hpp"

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
    while (this->commands[i].compare(message.getCommand()) && i < this->size)
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