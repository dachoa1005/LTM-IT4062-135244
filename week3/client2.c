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

    while (cmdEXIT == 0)
    {
        memset(buffer, 0, sizeof(buffer));
        recv(client_socket, buffer, sizeof buffer, 0);
        strcpy(buffer,trim(buffer));
        // printf("%s", buffer);
        // printf("%ld", strlen(buffer));
        if (strcmp(buffer, "") != 0)
        {
            printf("Received message: %s\n", buffer);
        }
        else
        {
            printf("exit\n");
            cmdEXIT = 1;
        }
    }
    return 0;
}