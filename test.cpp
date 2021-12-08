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

    while (1)
    {
        addr_len = sizeof(new_addr);
        new_fd = accept(fd, (struct sockaddr *) &new_addr, &addr_len);
        send(new_fd, "Hello world", 12, 0);
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
    printf("Caio\n");
    hints = ft_set_hints();
    printf("Sop\n");
    if (getaddrinfo(NULL, port, &hints, &addr))
        return (1);
    printf("Sempronio\n");
    fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (fd == -1)
        return (1);
    printf("Tullio\n");
    if (ft_bind(fd, addr))
        return (1);
    printf("Paolo\n");
    printf("%d\n", atoi(listen_port));
    if (listen(fd, atoi(listen_port)))
        return (1);
    printf("Fra\n");
    ft_loop(fd);
    return (0);
}