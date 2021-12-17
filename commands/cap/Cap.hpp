#ifndef CAP_HPP
#define CAP_HPP

#include "../ICommand.hpp"

#define DEL "\n"
#define DELSIZE 1

class Cap : public ICommand
{
    public:
        Cap();
        ~Cap();

        int    exec(Message message, Client *client, Server server);
    private:
        std::string setAnswer(std::string text, Client client) const;

        int    execList(Client *client, Server server);
        int    execReq(Message message, Client *client, Server server);
        int    execAck(Message message, Client *client);
        int    execNak(Message message, Client *client);
};

#endif