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
        if ('a' <= a[c] && a[c] <= 'z')
        {
            c++;
            continue;
        }
        else if ('A' <= a[c] && a[c] <= 'Z')
        {
            c++;
            continue;
        }
        else if ('0' <= a[c] && a[c] <= '9')
        {
            c++;
            continue;
        }
        else if (a[c] == ' ')
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

int main(int argc, char *argv[])
{
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
    struct sockaddr_in server_address;
    socklen_t addr_size;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_number);
    server_address.sin_addr.s_addr = inet_addr(IP_address);

    memset(server_address.sin_zero, '\0', sizeof server_address.sin_zero);
    addr_size = sizeof server_address;

    int connect_status = connect(client_socket, (struct sockaddr *)&server_address, addr_size);
    if (connect_status == -1)
    {
        printf("Connection failed\n");
        return 0;
    }
    else
    {
        printf("Connection established\n");
    }
    // printf("%d", cmdEXIT);
    // int cmdEXIT = 0;
    char buffer[1024] = "a";
    while (1)
    {
        if (strcmp(trim(buffer), "") != 0)
        {
            memset(buffer, 0, sizeof(buffer));
            // printf("Client 1 : ");
            // scanf until newline 
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strlen(buffer)-1] = '\0';
            // printf("%ld", strlen(buffer));
            send(client_socket, buffer, strlen(buffer), 0);
        }
        else
        {   
            printf("Exit\n");
            send(client_socket, buffer, strlen(buffer), 0);
            break;
        }
    }
    return 0;
}