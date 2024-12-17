#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <aio.h>
#include <signal.h>

char *socket_path = "socket";

struct aiocb *create_request(int socket) 
{
    struct aiocb *request = calloc(1, sizeof(struct aiocb));
    request->aio_fildes = socket;
    request->aio_buf = malloc(100);
    request->aio_nbytes = 100;
    request->aio_sigevent.sigev_notify = SIGEV_SIGNAL;
    request->aio_sigevent.sigev_signo = SIGIO;
    request->aio_sigevent.sigev_value.sival_ptr = request;
    return request;
}

void event_handler(int sig, siginfo_t *info, void *context) {
    if (sig != SIGIO || info->si_signo != SIGIO) return;
    struct aiocb *request = info->si_value.sival_ptr;
    if (aio_error(request) == 0) 
    {
        size_t size = aio_return(request);
        char *buffer = (char *) request->aio_buf;
        if (size == 0) 
        {
            printf("\nEOF reached, closing connection\n");
            close(request->aio_fildes);
            free(buffer);
            free(request);
            return;
        }
        for (int i = 0; i < size; i++) 
        {
            int c = toupper(buffer[i]);
            printf("%c", c);
        }
        aio_read(request);
    }
}

int main() 
{
    int fd, cl;
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) 
    {
        perror("socket error");
        exit(-1);
    }
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);
    unlink(socket_path);
    if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) 
    {
        perror("bind error");
        exit(-1);
    }
    if (listen(fd, 5) == -1) 
    {
        perror("listen error");
        exit(-1);
    }
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_sigaction = event_handler;
    action.sa_flags = SA_SIGINFO | SA_RESTART;
    sigaction(SIGIO, &action, NULL);
    printf("readyserver\n"); 
    while (1) 
    {
        if ((cl = accept(fd, NULL, NULL)) == -1)
        {
            continue;
        }
        else
        {
            printf("\nadd socket\n");
        }
        struct aiocb *request = create_request(cl);
        if (aio_read(request) == -1)
        {
            perror("aio_read error");
            exit(-1);
        }
    }
    exit(0);
}

