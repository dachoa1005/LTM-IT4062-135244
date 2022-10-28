#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>

char* trim(char* str)
{
    char* end;
    while(isspace(*str)) str++;
    if(*str == 0)  // All spaces?
        return "";
    end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) end--;
    // Write new null terminator character
    end[1] = '\0';
    return str;
}

int string_check(char a[]) // check if string contain character other than number or alphabet
{
    int c = 0;
    while (a[c] != '\0')
    {
        if ('a' <= c[a] && c[a] <= 'z')
        {
            c++;
            continue;
        }
        else if ('A' <= c[a] && c[a] <= 'Z')
        {
            c++;
            continue;
        }
        else if ('0' <= c[a] && c[a] <= '9')
        {
            c++;
            continue;
        }
        else if (c[a] == ' ')
        {
            c++;
            continue;
        }
        else
        {
            return -1;
        }
        return -1;
        c++;
    }
    return 0;
}

void split_string(char str[])
{
    int i=0;
    while(i < strlen(str))
    {
        if ('0' <= str[i] && str[i] <= '9')
        {

        }
    }
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
        printf("Usage ./server <Port number>\n");
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
        printf("Client 1 has connected\n");

    Client2 = accept(welcomeSocket, (struct sockaddr *)&serverStorage, &addr_size);
    if (Client2 != -1)
        printf("Client 2 has connected\n");

    int cmdEXIT = 0;
    while (cmdEXIT == 0)
    {
        memset(buffer, '\0', 1024);
        int bytes_recv = recv(Client1, buffer, 1024, 0);
        if (bytes_recv == -1)
        {
            printf("Receive failed\n");
            return 0;
        }
        else
        {
            printf("Client 1: %s \n", trim(buffer));
            // printf("%ld\n", strlen(buffer));
            if (strcmp((trim(buffer)),"") == 0)
            {
                printf("exit\n");
                send(Client2, buffer, strlen(buffer), 0);
                cmdEXIT = 1;
            }
            else
            {
                send(Client2, buffer, strlen(buffer), 0);
            }
        }
    }
    close(Client1);
    close(Client2);
    close(welcomeSocket);
    return 0;
}