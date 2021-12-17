/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 15:17:50 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/16 15:18:06 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

/**PUBBLIC_FUNCTIONS**/

Irc::Irc()
{
    std::cout << "New Irc" << std::endl;
}

Irc::~Irc()
{
    std::cout << "Irc deleted" << std::endl;
}

int Irc::startCommunication(int fdNewClient)
{
    int flag;
    Client  *client = new Client();
    std::vector<std::string> array;

    flag = 0;
    client->setSocketFd(fdNewClient);
    while (flag != 3 && flag != 7)
    {
        array = this->ft_take_messages(fdNewClient);
        for (std::vector<std::string>::iterator it = array.begin();it != array.end(); it++)
            flag = this->ft_exec_communication_commands(flag, *it, client);
    }
    this->server.setClient(client);
    ft_welcome(client);
    return (0);
}

void    Irc::printClients()
{
    std::vector<Client *>::iterator it;

    for (it = this->server.getClients().begin(); it != this->server.getClients().end(); it++)
        std::cout << "Nick: " << (*it)->getNickname() << std::endl;
}

/**PRIVATE_FUNCTIONS**/

std::vector<std::string> Irc::ft_take_messages(int fdNewClient)
{
    char *buffer = new char[512]();
    std::string b;
    std::vector<std::string> array;

    b = "";
    recv(fdNewClient, buffer, 512, 0);
    this->ft_parse_data(&array, &b, buffer);
    ft_memset(buffer, 512);
    while (b != "")
    {
        recv(fdNewClient, buffer, 512, 0);
        this->ft_parse_data(&array, &b, buffer);
        ft_memset(buffer, 512);  
    }
    delete [] buffer;
    return (array);
}

void Irc::ft_parse_data(std::vector<std::string> *array, std::string *b, char *buffer)
{
    int num;
    int lastIndx;
    std::string tmp = "";

    num = 0;
    lastIndx = 0;
    tmp.append(*b);
    *b = "";
    tmp.append(buffer);
    num = tmp.find(DEL);
    while (num != -1)
    {
        array->push_back(tmp.substr(lastIndx, num - lastIndx + DELSIZE + 1));
        lastIndx = num + DELSIZE;
        num = tmp.find(DEL, lastIndx);
    }
    if (lastIndx + 1 != tmp.size())
        b->append(tmp.substr(lastIndx, tmp.size() - lastIndx));
}

int Irc::ft_exec_communication_commands(int flag, std::string text, Client *client)
{
    RepliesCreator  reply;
    Message message;
    CommandCreator    cCreator;
    std::string commands[5] = {
        "CAP",
        "NICK",
        "PASS",
        "PING",
        "USER"
    };
    int i;
    int num;

    i = 0;
    message.setMessage(text);
    ICommand *command = cCreator.makeCommand(message);
    if (!command)
        return (1);
    while (i < 5 && command->getCommand().compare(commands[i]))
        i++;
    switch (i)
    {
        case (0):                                   //CAP
            //return (flag);
            if ((flag & 4) == 0 || (flag & 4) == 4)
            {
                num = command->exec(message, client, this->server);
                if (num == 0)
                    return (flag | 4);
                else if (num == 2 && (flag & 3) == 3)
                    return (7);
            }
        case (1):                                   //NICK
            command->exec(message, client, this->server);
            return (flag | 2);
        case (2):                                    //PASS
            if (!(flag & 3))
                command->exec(message, client, this->server);
            else
                reply.makeErrorAlreadyRegistered(client->getNickname());
            return (flag);
        case (3):
            command->exec(message, client, this->server);
            return (flag);
        case (4):                                    //USER
            num = command->exec(message, client, this->server);
            if (num == 1)
                return (flag | 1);
    }
    return (flag);
}

int Irc::ft_welcome(Client *client)
{
    RepliesCreator  reply;
    std::string text;

    text = reply.makeWelcome(client->getNickname(), client->getUsername(), this->server.getServername());
    text.append(reply.makeYourHost(this->server.getServername(), this->server.getVersion(), client->getNickname()));
    text.append(reply.makeCreated(this->server.getDate(), client->getNickname()));
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return(0);
}

void Irc::ft_memset(char *buffer, int size)
{
    for (int i = 0; i < size ;i++)
        buffer[i] = 0;
}
