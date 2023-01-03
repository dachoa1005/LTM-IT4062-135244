#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024 // Kích thước buffer

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[BUFFER_SIZE];

    // Kiểm tra số lượng tham số truyền vào
    if (argc < 3)
    {
        fprintf(stderr, "ERROR, no hostname or port provided\n");
        exit(1);
    }

    // Đọc số hiệu cổng từ dòng lệnh
    portno = atoi(argv[2]);

    // Tạo socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }

    // Xóa bộ nhớ cho địa chỉ server
    memset((char *)&serv_addr, 0, sizeof(serv_addr));

    // Cấu hình thông tin cho địa chỉ server
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    // Kết nối tới server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR connecting");
        exit(1);
    }

    // Lặp vô hạn để nhập và gửi dữ liệu tới server
    while (1)
        {
            char username[100];
            char password[100];

            memset(buffer, 0, BUFFER_SIZE);
            printf("Enter username: ");
            fgets(username, 100, stdin);
            username[strlen(username) - 1] = '\0';
            // getchar();

            printf("Enter password: ");
            fgets(password, 100, stdin);
            password[strlen(password) - 1] = '\0';
            // getchar();

            strcat(buffer, username);
            strcat(buffer, " ");
            strcat(buffer, password);

            printf("Sending message: %s\n", buffer);
            n = send(sockfd, buffer, BUFFER_SIZE, 0);

            if (n < 0)
            {
                perror("ERROR writing to socket");
                break;
            }

            memset(buffer, 0, BUFFER_SIZE);
            n = recv(sockfd, buffer, BUFFER_SIZE, 0);
            if (n < 0)
            {
                perror("ERROR reading from socket");
                break;
            }
            // printf("Received message: %s\n", buffer);
            if (strcmp(buffer, "LOCK") == 0 || strcmp(buffer, "SUCC") == 0)
            {
                if (strcmp(buffer, "LOCK") == 0)
                {
                    printf("Account is locked\n");
                }
                else
                {
                    printf("Login successfully\n");
                }
                break;
            } else if (strcmp(buffer, "FAIL") == 0) //nhập lại username và password
            {
                printf("Login failed, re-enter username and password\n");
            }
        } 
        close(sockfd);

        return 0;
    }
