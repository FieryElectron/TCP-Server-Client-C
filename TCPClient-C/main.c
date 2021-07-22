#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


#define PORT 8080
#define BUF_LEN 1024

int main(int argc, char const *argv[])
{
    int server_fd = 0, recv_len;
    struct sockaddr_in server_address;

    char buffer[BUF_LEN] = {0};

    // Create server socket | AF_INET => IPv4 protocol | SOCK_STREAM => TCP
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed\n");
        return -1;
    }

    // Set socket attributes
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IPv4/6 string addresses to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0)
    {
        perror("invalid address\n");
        return -1;
    }

    // Connect to server
    if (connect(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("connect failed\n");
        return -1;
    }

    // Communication logic
    int c;

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        printf("\033[33mClient: ");
        scanf("%[^\n]s", buffer);

        while ((c = getchar()) != '\n' && c != EOF);
        
        send(server_fd, buffer, strlen(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
        recv_len = read(server_fd, buffer, BUF_LEN);
        if(recv_len == 0){
            break;
        }
        printf("\033[36mServer: %s\n", buffer);
    }

    close(server_fd);

    return 0;
}
