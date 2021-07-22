#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080
#define BUF_LEN 1024
#define BACKLOG 1

int main(int argc, char const *argv[])
{
    int server_fd, client_fd, recv_len;
    struct sockaddr_in client_address;
    int client_address_len = sizeof(client_address);
    int sockopt = 1;

    char buffer[BUF_LEN] = {0};
    char Tbuffer[BUF_LEN] = {0};

    // Create server socket | AF_INET => IPv4 protocol | SOCK_STREAM => TCP
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed\n");
        return -1;
    }

    // Set server socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &sockopt, sizeof(sockopt)) < 0)
    {
        perror("setsockopt failed\n");
        return -1;
    }

    // Set socket attributes
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = INADDR_ANY; // ANY incoming addr
    client_address.sin_port = htons(PORT);

    // Bind sockets
    if (bind(server_fd, (struct sockaddr *)&client_address, sizeof(client_address)) < 0)
    {
        perror("bind failed\n");
        return -1;
    }

    // Set Max listen BACKLOG
    if (listen(server_fd, BACKLOG) < 0)
    {
        perror("listen failed\n");
        return -1;
    }

ACCEPT:
    printf("\033[35mWaiting for new connection...\n");
    // Wait for connection
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t *)&client_address_len)) < 0)
    {
        perror("accept failed\n");
        return -1;
    }

    // Communication logic
    while(1){
        memset(buffer, 0, sizeof(buffer));
        memset(Tbuffer, 0, sizeof(Tbuffer));
        recv_len = read(client_fd, buffer, BUF_LEN);
        printf("\033[33mClient: %s\n",buffer);

        if(strcmp(buffer, "Bye") == 0){
            break;
        }

        if(strcmp(buffer, "Shutdown") == 0){
            goto EXIT;
        }

        memcpy(Tbuffer, buffer, sizeof(buffer));
        memset(buffer, 0, sizeof(buffer));

        strcat(buffer, "U said=> ");
        strcat(buffer, Tbuffer);

        printf("\033[36mServer: %s\n",buffer);
        send(client_fd, buffer, strlen(buffer), 0);
    }

    close(client_fd);
    printf("\033[35mDisconnected\n");
    goto ACCEPT;
EXIT:

    printf("\033[35mServer closed\n");

    return 0;
}
