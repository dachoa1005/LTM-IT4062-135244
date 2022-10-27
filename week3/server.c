#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int compare_strings(char a[], char b[])
{
    int c = 0;
    while (a[c] == b[c]) 
    {
        if (a[c] == '\0' || b[c] == '\0')
        break;
        c++;
    }
    if (a[c] == '\0' && b[c] == '\0')
    return 0;
    else
    return -1;
    // fsdfhskjdhfkj\0asdahdskjh
    // fsdfhskjdhfkj\0asdqweqwe
}

int main(int argc, char *argv[]) {
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
    bind(welcomeSocket, (struct sockaddr *) &server_addr, sizeof(server_addr));

    if (listen(welcomeSocket,5)==0)
        printf("Listening\n");
    else
        printf("Error\n");

    addr_size = sizeof serverStorage;
    Client1 = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
    Client2 = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

    int cmdEXIT = 0;
    while (cmdEXIT == 0)
    {
        recv(Client1, buffer, 1024, 0);
        printf ("\nMessage from client 1: %s\n", buffer);
        send(Client2,buffer,1024,0);
        if (compare_strings(buffer, "exit")==0)
        {   
            cmdEXIT = 1;
        }
        else 
        {
            memset(&buffer[0], 0, sizeof(buffer));
            recv(Client2, buffer, 1024, 0);
            printf ("\nMessage from client 2: %s\n", buffer);
            send(Client1,buffer,1024,0);
            if (compare_strings(buffer, "exit")==0)
            {
                cmdEXIT = 1;
            }
        }
    }

    return 0;
}