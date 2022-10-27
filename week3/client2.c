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
        port_number = atoi(argv[2]);
        strcpy(IP_address, argv[1]);
    }
    else
    {
        printf("Usage ./client  <IP address> <Port number>\n");
        return 0;
    }
    int client_socket;
    char buffer[1024];
    struct sockaddr_in server_address;
    socklen_t addr_size;
    int cmdEXIT = 0;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_number);
    server_address.sin_addr.s_addr = inet_addr(IP_address);
    
    memset(server_address.sin_zero, '\0', sizeof server_address.sin_zero);
    addr_size = sizeof server_address;

    int connect_status = connect(client_socket, (struct sockaddr *) &server_address, addr_size);
    if (connect_status == -1)
    {
        printf("Connection failed\n");
        return 0;
    }
    else
    {
        printf("Connection established\n");
    }

    while (cmdEXIT == 0)
    {
        int recvValue = recv(client_socket, buffer, sizeof buffer - 1, 0);
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
            send(client_socket,buffer,sizeof buffer - 1,0);
            if (compare_strings(buffer, "exit")==-1)
            {
                memset(&buffer[0], 0, sizeof(buffer));
            }
            else cmdEXIT = 1;
        }   
    }
    return 0;
}
