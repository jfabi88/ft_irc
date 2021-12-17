#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "../ICommand.hpp"

class Privmsg : public ICommand
{
    public:
        Privmsg();
        ~Privmsg();

        int    exec(Message message, Client *client, Server server);
    private:
        std::string setAnswer(std::string text, Client client) const;
};

#endif
