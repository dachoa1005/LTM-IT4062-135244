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
    char username[100];
    char password[100];
    int status;
    char message[200];
    while (1)
    {
        printf("Enter username: ");
        fgets(username, sizeof(username), stdin);
        username[strlen(username)-1] = '\0';

        printf("Enter password: ");
        fgets(password, sizeof(password), stdin);
        password[strlen(password)-1] = '\0';

        strcpy(message, username);
        strcat(message, " ");
        strcat(message, password);
        message[strlen(message)] = '\0';

        send(client_socket, message, 200, 0);
        memset(buffer, 0, sizeof(buffer));
        recv(client_socket, buffer, 1024, 0);
        printf("%s\n", buffer);
    }
    return 0;
}