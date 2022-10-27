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
}

int main(int argc, char *argv[]) {
    int port_number;
    char IP_address[20];

    if (argc == 3)
    {
        port_number = atoi(argv[1]);
        strcpy(IP_address, argv[2]);
    }
    else
    {
        printf("Usage ./client  <IP address> <Port number>\n");
        return 0;
    }

    int clientSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    int cmdEXIT = 0;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port_number);
    serverAddr.sin_addr.s_addr = inet_addr(IP_address);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    addr_size = sizeof serverAddr;

    connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

    while (cmdEXIT == 0)
    {
        int recvValue = recv(clientSocket, buffer, sizeof buffer - 1, 0);
        if (recvValue != 1)
        {
            if (compare_strings(buffer, "exit")==-1)
            {
                printf("Client 1 : ");
                printf("%s\n", buffer);
                memset(&buffer[0], 0, sizeof(buffer));
            }
            else cmdEXIT = 1;
        }
        else
        {
            printf("Client 2 : ");
            scanf(" %[^\n]s", buffer);
            send(clientSocket,buffer,sizeof buffer - 1,0);
            if (compare_strings(buffer, "exit")==-1)
            {
                memset(&buffer[0], 0, sizeof(buffer));
            }
            else cmdEXIT = 1;
        }   
    }
    return 0;
}
