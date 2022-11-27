#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct User
    {
        char username[100];
        char password[100];
        int status;
    };

    struct User user[100];

    // read file account.txt and save into user struct
    FILE *file;
    file = fopen("account.txt", "r");
    char line[100];
    int i = 0;
    while (fgets(line, 100, file)!=NULL)
    {
        //printf("%s", line);
        strcpy(user[i].username, strtok(line, " "));
        strcpy(user[i].password, strtok(NULL, " "));
        user[i].status = atoi(strtok(NULL, " "));
        printf("%s %s %d\n", user[i].username, user[i].password, user[i].status);
        i++;    
    }
    int arr_user_size = i;

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
        printf("Client has connected\n");
    
    while (1)
    {
        recv(Client1, buffer, 100, 0);
        printf("Received: %s\n", buffer);
        char temp_username[100];
        char temp_password[100];
        strcpy(temp_username, strtok(buffer, " "));
        printf("%s\n", temp_username);
        strcpy(temp_password, strtok(NULL, " "));
        printf("%s\n", temp_password);
        for (i=0; i< arr_user_size; i++)
        {
            if (strcmp(user[i].username, temp_username) == 0)
            {
                if (strcmp(user[i].password, temp_password) == 0)
                {
                    if (user[i].status == 1)
                    {
                        send(Client1, "Login success", 100, 0);
                        break;
                    }
                    else if (user[i].status == 0)
                    {
                        send(Client1, "Account is locked", 100, 0);
                        break;
                    }
                }
                else 
                {
                    send(Client1, "Wrong password", 100, 0);
                    break;
                }
            } else if (i == arr_user_size - 1)
            {
                send(Client1, "Account not found", 100, 0);
            }   
        }
    }
    close(Client1);
    close(welcomeSocket);
    return 0;
}