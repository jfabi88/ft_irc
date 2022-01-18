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

#include "srcs/Server.hpp""

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
    int             fdmax;
    int             new_fd;
    int             nBytes;
    fd_set          master;
    fd_set          read_fds;
    struct sockaddr_in new_addr;
    socklen_t       addr_len;
    void            *buffer;
    int             i;
    int             j;
    Server          irc;

    buffer = malloc(512);
    if (listen(fd, 10) == -1) {
        perror("listen");
        exit(3);
    }
    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    FD_SET(fd, &master);
    fdmax = fd;
    while (1)
    {
        read_fds = master;
        std::cout << "Prima del select" << std::endl;
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
        {
            perror("select");
            exit(4);
        }
        std::cout << "Dopo il select" << std::endl;
        for (i = 0; i <= fdmax; i++)
        {
            std::cout << "ciclo" << std::endl;
            if (FD_ISSET(i, &read_fds))
            {
                std::cout << "Dopo if" << std::endl;
                if (i == fd)
                {
                    addr_len = sizeof(new_addr);
                    std::cout << "Nuova connesione" << std::endl;
                    new_fd = accept(fd, (struct sockaddr *) &new_addr, &addr_len);
                    irc.startCommunication(new_fd);
                    if (new_fd!= -1)
                    {
                        FD_SET(new_fd, &master);
                        if (new_fd > fdmax)
                            fdmax = new_fd;
                    }
                }
            }
            else
            {
                std::cout << "Dentro else" << std::endl;
                if ((nBytes = recv(i, buffer, sizeof(buffer), 0)) <= 0)
                {
                    std::cout << "Dentro if: " << i << std::endl;
                    close (i);
                    std::cout << "Siamo dopo l'FD" << std::endl;
                    FD_CLR(i, &master);
                    std::cout << "Siamo dopo l'FD" << std::endl;
                }
                else
                {
                    for (j = 0; i <= fdmax; j++)
                    {
                        if (FD_ISSET(j, &master))
                        {
                            if (j!= fd && j!= i)
                            {
                                send(j, buffer, nBytes, 0);
                            }
                        }
                    }
                }
            }
        }
        irc.printClients();
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