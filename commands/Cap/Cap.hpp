#ifndef CAP_HPP
#define CAP_HPP

#include "../ICommand.hpp"

class Cap : public ICommand
{
    public:
        Cap(Message message, Server server, Client client);
        ~Cap();

        void    exec() const;
    private:
        std::string setAnswer(std::string text, Client client) const;
        std::string array[];

        void        execList();
        void        execReq();
};

#endif