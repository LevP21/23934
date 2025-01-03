#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "unix_socket"
#define BUFFER_SIZE 1024

int main()
{
    int client_fd;
    struct sockaddr_un server_addr;

    // Создание сокета
    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Подключение к серверу
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connect");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];

    // Отправка текста серверу
    printf("Enter text to send (type 'exit' to quit):\n");
    while (fgets(buffer, sizeof(buffer), stdin))
    {
        if (strncmp(buffer, "exit", 4) == 0)
        {
            break;
        }

        if (write(client_fd, buffer, strlen(buffer)) == -1)
        {
            perror("write");
            close(client_fd);
            exit(EXIT_FAILURE);
        }
    }

    close(client_fd);
    printf("Client terminated.\n");
    return 0;
}
