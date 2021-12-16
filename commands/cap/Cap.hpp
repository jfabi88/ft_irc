#ifndef CAP_HPP
#define CAP_HPP

#include "../ICommand.hpp"

class Cap : public ICommand
{
    public:
        Cap();
        ~Cap();

        void    exec(Message message, Client client, Server server);
    private:
        std::string setAnswer(std::string text, Client client) const;

        void    execList(Client client, Server server);
        void    execReq(Message message, Client client, Server server);
        void    execAck(Message message, Client client);
        void    execNak(Message message, Client client);
};

#endif