#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "../ICommand.hpp"

class Privmsg : public ICommand
{
    public:
        Privmsg(Message message, Server server, Client client);
        ~Privmsg();

        void    exec() const;
    private:
        std::string setAnswer(std::string text, Client client) const;
};

#endif
