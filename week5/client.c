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
        printf("Usage %s  <IP address> <Port number>\n", argv[0]);
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
    int choice;
    char buffer[1024];
    while (choice != 3)
    {
        printf("\n_____________________________________\n");
        printf("Menu:\n");
        printf("1. Send message\n");
        printf("2. Send file's content\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();
        // printf("You entered: %d", choice);
        switch (choice)
        {
        case 1:
        {
            send(client_socket, &choice, sizeof(choice), 0);
            while (1)
            {
                memset(buffer, '\0', sizeof(buffer));
                printf("Enter message: ");
                // fflush(stdin);
                scanf("%[^\n]%*c", buffer);
                // printf("You entered: %s", buffer);
                // buffer[strlen(buffer) - 1] = '\0';
                if (strcmp((trim(buffer)), "") == 0)
                {
                    printf("Break\n");
                    send(client_socket, buffer, 1024, 0);
                    break;
                }
                else
                {
                    send(client_socket, buffer, 1024, 0);
                    memset(buffer, '\0', 1024);
                }
            }

            break;
        }
        case 2:
        {
            send(client_socket, &choice, sizeof(choice), 0);
            int total_bytes_sent = 0;
            int bytes_sent = 0;
            int file_size = 0;
            char file_path[1024];
            char buffer[1024];
            printf("Enter file path: ");
            scanf("%[^\n]%*c", file_path);
            FILE *fp = fopen(file_path, "r");
            
            // get file size
            fseek(fp, 0, SEEK_END);
            file_size = ftell(fp);
            rewind(fp);

            // send file size
            send(client_socket, &file_size, sizeof(file_size), 0);

            while (total_bytes_sent < file_size)
            {
                if (file_size - total_bytes_sent < 1024)
                {
                    bytes_sent = fread(buffer, 1, file_size - total_bytes_sent, fp);
                    send(client_socket, buffer, bytes_sent, 0);
                    total_bytes_sent += bytes_sent;
                }
                else
                {
                    bytes_sent = fread(buffer, 1, 1024, fp);
                    send(client_socket, buffer, bytes_sent, 0);
                    total_bytes_sent += bytes_sent;
                }
            }
            break;
        }
        case 3:
        {
            send(client_socket, &choice, sizeof(choice), 0);
            close(client_socket);
            close(server_address.sin_port);
            break;
        }
        default:
        {
            printf("Invalid command\n");
            break;
        }
        }
    }
    printf("Connection closed\n");
    return 0;
}