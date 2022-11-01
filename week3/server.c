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

void split_string(char* str)
{
    char* str_num;
    str_num = malloc(strlen(str));
    char* str_char;
    str_char = malloc(strlen(str));
    int i=0;
    while(i < strlen(str))
    {
        if ('0' <= str[i] && str[i] <= '9')
        {
            char temp = str[i];
            strncat(str_num, &temp, 1);
        } else if ('a' <= str[i] && str[i] <= 'z' || 'A' <= str[i] && str[i] <= 'Z')
        {
            char temp = str[i];
            strncat(str_char, &temp, 1);
        }
        i++;
    }
    char *result = malloc(sizeof(char) * (strlen(str_num) + strlen(str_char) + 1));
    strcpy(result, str_num);
    char temp = '\n';
    if (strcmp(str_num,"") != 0)
    {
        if (strcmp(str_char,"") != 0)
        {
            strncat(result, &temp, 1);
        }
    }
    strcat(result, str_char);
    strcpy(str, result);
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

    
    while (1)
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
            // printf("Client 1: %s \n", buffer);
            
            // printf("%ld\n", strlen(buffer));
            if (strcmp((trim(buffer)),"") == 0)
            {
                printf("Exit\n");
                send(Client2, buffer, strlen(buffer), 0);
                break;
            } 
            else if (string_check(buffer) == -1)
            {
                printf("Message Error\n");
                send(Client2, "Error", strlen("Error"), 0);
            }
            else
            {
                char *temp_buffer = malloc(strlen(buffer));
                strcpy(temp_buffer, buffer);
                split_string(temp_buffer);
                printf("%s\n", temp_buffer);
                send(Client2, temp_buffer, strlen(temp_buffer), 0);
            }
        }
    }
    close(Client1);
    close(Client2);
    close(welcomeSocket);
    return 0;
}