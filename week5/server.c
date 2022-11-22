#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>

char *trim(char *str)
{
    char *end;
    while (isspace(*str))
        str++;
    if (*str == 0) // All spaces?
        return "";
    end = str + strlen(str) - 1;
    while (end > str && isspace(*end))
        end--;
    // Write new null terminator character
    end[1] = '\0';
    return str;
}

int main(int argc, char *argv[])
{
    int port_number;
    if (argc == 2)
    {
        port_number = atoi(argv[1]);
    }
    else
    {
        printf("Usage %s <Port number>\n", argv[0]);
        return 0;
    }

    int welcomeSocket, Client1, Client2;
    struct sockaddr_in server_addr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    char buffer[1024];
    welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    memset(server_addr.sin_zero, '\0', sizeof server_addr.sin_zero);
    bind(welcomeSocket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if (listen(welcomeSocket, 5) == 0)
        printf("Listening\n");
    else
        printf("Error\n");

    addr_size = sizeof serverStorage;
    Client1 = accept(welcomeSocket, (struct sockaddr *)&serverStorage, &addr_size);
    if (Client1 != -1)
        printf("Client has connected\n");

    int cmd = 0;

    while (cmd != 3)
    {
        printf("Waiting for command from client\n");
        recv(Client1, &cmd, sizeof(cmd), 0);
        printf("cmd: %d\n", cmd);
        switch (cmd)
        {
        case 1:
        {
            printf("Recieve message from client\n");
            while (1)
            {
                memset(buffer, '\0', 1024);
                int bytes_recv = recv(Client1, buffer, 1024, 0);
                if (bytes_recv == -1)
                {
                    printf("Receive failed\n");
                    break;
                }
                else
                {
                    printf("Client: %s \n", buffer);
                    // printf("%ld\n", strlen(buffer));
                    if (strcmp((trim(buffer)), "") == 0)
                    {
                        printf("Break\n");
                        break;
                    }
                }
            }
            break;
        }
        case 2:
        {
            int total_bytes_recv = 0;
            int bytes_recv = 0;
            int file_size = 0;

            printf("Recieve file from client\n");
            // Recieve file size
            recv(Client1, &file_size, sizeof(file_size), 0);
            printf("File size: %d\n", file_size);
            // Recieve file
            while (total_bytes_recv < file_size)
            {
                memset(buffer, '\0', 1024);
                bytes_recv = recv(Client1, buffer, 1024, 0);
                if (bytes_recv == -1)
                {
                    printf("Receive failed\n");
                    break;
                }
                total_bytes_recv += bytes_recv;
                printf("%s", buffer);
            }
            printf("\n");
            break;
        }
        case 3:
        {
            close(Client1);
            close(welcomeSocket);
            close(Client2);
            printf("Close connection\n");
            break;
        }
        default:
            break;
        }
    }

close(Client1);
close(welcomeSocket);
return 0;
}