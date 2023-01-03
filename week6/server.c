#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BACKLOG 10       // Số lượng kết nối đợi tối đa
#define BUFFER_SIZE 1024 // Kích thước buffer

typedef struct
{
    char username[20];
    char password[20];
    int status;
} User;

int main(int argc, char *argv[])
{
    int sockfd, new_sockfd, portno;
    socklen_t clilent;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[BUFFER_SIZE];
    int n;

    // Kiểm tra số hiệu cổng nhập từ dòng lệnh
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    // Tạo socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }

    // Xóa bộ nhớ cho địa chỉ server
    memset((char *)&serv_addr, 0, sizeof(serv_addr));

    // Đọc số hiệu cổng từ dòng lệnh
    portno = atoi(argv[1]);

    // Cấu hình thông tin cho địa chỉ server
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Gán socket với số hiệu cổng
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        exit(1);
    }

    // Cho phép server lắng nghe kết nối từ client
    listen(sockfd, BACKLOG);

    // Lặp vô vô tuyến để chấp nhận kết nối từ các client
    while (1)
    {
        // Chấp nhận kết nối từ client và tạo một socket mới cho kết nối này
        clilent = sizeof(cli_addr);
        new_sockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilent);
        if (new_sockfd < 0)
        {
            perror("ERROR on accept");
            continue;
        }
        else
        {
            printf("New client connected\n");
        }
        // Tạo một tiến trình con mới cho kết nối từ client này
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("ERROR on fork");
            close(new_sockfd);
            continue;
        }

        // Trong tiến trình con, nhận dữ liệu từ client và gửi lại cho client
        if (pid == 0)
        {
            close(sockfd); // Đóng socket của tiến trình cha
            while (1)
            {
                // lưu dữ liệu file account.txt vào mảng
                FILE *f = fopen("account.txt", "r");
                char line[255];
                int i = 0;
                User user[100];
                char *token;
                char temp_username[100];
                char temp_password[100];

                while (fgets(line, 100, f) != NULL)
                {
                    // printf("%s", line);
                    strcpy(user[i].username, strtok(line, " "));
                    strcpy(user[i].password, strtok(NULL, " "));
                    user[i].status = atoi(strtok(NULL, " "));
                    // printf("%s %s %d\n", user[i].username, user[i].password, user[i].status);
                    i++;
                }
                // printf("File account.txt read successfully.\n");
                // Nhận dữ liệu từ client

                if (n < 0)
                {
                    perror("ERROR reading from socket");
                    break;
                }

                int check_time = 0; // kiểm tra số lần nhập sai
                while (check_time < 3)
                {
                    memset(buffer, 0, BUFFER_SIZE);
                    n = recv(new_sockfd, buffer, BUFFER_SIZE, 0);
                    if (strcmp(buffer, "")!=0)
                    {
                        printf("Received message: %s\n", buffer);
                    }
                    token = strtok(buffer, " ");
                    strcpy(temp_username, token);
                    token = strtok(NULL, " ");
                    strcpy(temp_password, token);

                    for (int i = 0; i < 100; i++)
                    {
                        if (strcmp(user[i].username, temp_username) == 0 && strcmp(user[i].password, temp_password) == 0)
                        {
                            if (user[i].status == 0)
                            {
                                strcpy(buffer, "LOCK");
                                check_time = 3;
                                printf("User %s is locked.\n", temp_username);
                                break;
                            }
                            else
                            {
                                strcpy(buffer, "SUCC");
                                check_time = 3;
                                printf("User %s is logged in.\n", temp_username);
                                break;
                            }
                            send(new_sockfd, buffer, BUFFER_SIZE, 0);
                            printf("Sent message: %s\n", buffer);
                            break;
                        }
                        else
                        {
                            strcpy(buffer, "FAIL");
                            check_time++;
                            if (check_time == 3)
                            {
                                printf("User %s is locked.\n", temp_username);
                                strcpy(buffer, "LOCK");
                            }

                            send(new_sockfd, buffer, BUFFER_SIZE, 0);
                            break;
                        }
                    }
                }

                n = send(new_sockfd, buffer, BUFFER_SIZE, 0);
                if (n < 0)
                {
                    perror("ERROR writing to socket");
                    break;
                }
            }
            close(new_sockfd);
            exit(0);
        }
        else
        {
            // Trong tiến trình cha, đóng socket của tiến trình con và tiếp tục chấp nhận kết nối từ các client khác
            close(new_sockfd);
        }
    }

    close(sockfd);

    return 0;
}