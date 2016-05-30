
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int net_create_listener(char *addr, short port)
{
    int fd;
    int flags;
    struct linger ln = {0, 0};
    struct sockaddr_in si;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        return -1;
    }

    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(flags));
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &flags, sizeof(flags));
    setsockopt(fd, SOL_SOCKET, SO_LINGER, &ln, sizeof(ln));
#if !defined(TCP_NOPUSH) && defined(TCP_NODELAY)
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &flags, sizeof(flags));
#endif

    si.sin_family = AF_INET;
    si.sin_port = htons(port);
    si.sin_addr.s_addr = htonl(inet_addr(addr));

    if (bind(fd, (struct sockaddr *)&si, sizeof(si)) == -1) {
        goto error;
    }

    if (listen(fd, 1024) == -1) {
        goto err;
    }

    return fd;

err:
    close(fd);
    return -1;
}
