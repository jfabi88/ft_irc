#include <string.h>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "Message.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "commands/CommandCreator.hpp"
#include "ICommand.hpp"
#include "Privmsg.hpp"
#include "RepliesCreator.hpp"

struct addrinfo ft_set_hints(void)
{
    struct addrinfo hints;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    return (hints);
}

int    ft_control(int argc, char *argv[])
{
    if (argc != 4)
        return (1);
    return (0);
}

int ft_bind(int fd, struct addrinfo *addr)
{
    int reuse = 1;

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)))
        return (1);
    if (bind(fd, addr->ai_addr, addr->ai_addrlen))
        return (1);
    return (0);
}

int ft_loop(int fd)
{
    int             new_fd;
    struct sockaddr_in new_addr;
    socklen_t       addr_len;
    void            *buffer;
    Server          server;
    RepliesCreator  replies;

    buffer = malloc(512);
    memset(buffer, 0, 512);
    while (1)
    {
        addr_len = sizeof(new_addr);
        new_fd = accept(fd, (struct sockaddr *) &new_addr, &addr_len);
        Client primo;
        primo.setNickname("pollo");
        primo.setSocketFd(new_fd);
        printf("Ciao\n");
        replies.sendReplies(1, server, primo);
        printf("Pollo\n");
        server.setClient(primo);
        recv(new_fd, buffer, 512, 0);
        for (int i = 0; i < 512; i++)
            printf("%c", ((char*)(buffer))[i]);
        Message mex((char*)(buffer));

        std::cout << mex << std::endl;
        close(new_fd);
    }
    return (0);
}

int main(int argc, char *argv[])
{
    const char      *host;
    const char      *port;
    const char      *listen_port;
    int             fd;
    struct addrinfo hints;
    struct addrinfo *addr;

    if (ft_control(argc, argv))
        return (1);
    host = argv[1];
    port = argv[2];
    listen_port = argv[3];
    hints = ft_set_hints();
    if (getaddrinfo(NULL, port, &hints, &addr))
        return (1);
    fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (fd == -1)
        return (1);
    if (ft_bind(fd, addr))
        return (1);
    if (listen(fd, atoi(listen_port)))
        return (1);
    ft_loop(fd);
    return (0);
}