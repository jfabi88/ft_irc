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
    Client  client;
    std::vector<std::string> array;

    flag = 1;
    client.setSocketFd(fdNewClient);
    while (flag)
    {
        array = this->ft_take_messages(fdNewClient);
        for (std::vector<std::string>::iterator it = array.begin();it != array.end(); it++)
            flag = this->ft_exec_communication_commands(flag, *it, client);
    }
    return (0);
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
    std::cout << "Finito" << std::endl;
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
        array->push_back(tmp.substr(lastIndx, num - lastIndx + 3));
        lastIndx = num + 2;
        num = tmp.find(DEL, lastIndx);
    }
    if (lastIndx + 1 != tmp.size())
        b->append(tmp.substr(lastIndx, tmp.size() - lastIndx));
}

int Irc::ft_exec_communication_commands(int flag, std::string text, Client client)
{
    RepliesCreator  reply;
    Message message;
    CommandCreator    cCreator;
    std::string commands[4] = {
        "CAP",
        "NICK",
        "PASS",
        "USER"
    };
    int i;

    i = 0;
    message.setMessage(text);
    ICommand *command = cCreator.makeCommand(message);
    if (!command)
        return (1);
    std::cout << "Il testo dentro exec: " << command->getCommand() << std::endl;
    while (i < 4 && command->getCommand().compare(commands[i]))
        i++;
    std::cout << i << std::endl;
    switch (i)
    {
        case (1):                                   //NICK
            command->exec(message, client, this->server);
            return (2);
        case(2):                                    //PASS
            if (flag != 2)
                command->exec(message, client, this->server);
            else
                reply.makeErrorAlreadyRegistered(client.getNickname());
            return (flag);
        case(3):                                    //USER
            command->exec(message, client, this->server);
            return (2);
    }
    return (flag);
}

void Irc::ft_memset(char *buffer, int size)
{
    for (int i = 0; i < size ;i++)
        buffer[i] = 0;
}
