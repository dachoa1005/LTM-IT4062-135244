#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <ctype.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

#define LISTENQ 100
#define BUFF_SIZE 1024

// handler process signal
void sig_chld(int signo);

void echo(int sockfd);

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <Port> \n", argv[0]);
        return -1;
    }
    int port = atoi(argv[1]);
    int listen_sock, conn_sock;
    struct sockaddr_in server, client;
    pid_t pid;
    socklen_t sin_size;

    if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Create socket failed!\n");
        return 0;
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    if (bind(listen_sock, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("Bind error: ");
        return 0;
    }

    if (listen(listen_sock, LISTENQ) == -1)
    {
        perror("Listen error: ");
        return 0;
    }

    printf("Folking server starts listening...\n\n");

    // establish a signal handler to catch SIGCHLD
    signal(SIGCHLD, sig_chld);

    while (1)
    {
        sin_size = sizeof(struct sockaddr_in);
        if ((conn_sock = accept(listen_sock, (struct sockaddr *)&client, &sin_size)) == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                perror("Accept error: ");
                return 0;
            }
        }

        // for each client, fork spawns a child, and the child handles the new client
        pid = fork();

        if (pid == 0)
        {
            // close(listen_sock);
            printf("You got a connection from %s\n", inet_ntoa(client.sin_addr));
            echo(conn_sock);
            exit(0);
        }

        // the parent closes the connected socket since the child handles the new client
        close(conn_sock);
    }

    close(listen_sock);
    return 0;
}

void sig_chld(int signo)
{
    pid_t pid;
    int stat;

    // wait the child process terminate
    pid = waitpid(-1, &stat, WNOHANG);
    printf("Child %d terminated.\n", pid);
}

void echo(int sockfd)
{
    char buff[BUFF_SIZE];
    int bytes_sent, bytes_received;

    for (;;)
    {
        bytes_received = recv(sockfd, buff, BUFF_SIZE, 0);
        if (bytes_received < 0)
        {
            perror("Receive error: ");
            break;
        }
        else if (bytes_received == 0)
        {
            printf("Connection closed.\n");
            break;
        }

        bytes_sent = send(sockfd, buff, bytes_received, 0);
        if (bytes_sent < 0)
        {
            perror("Send error: ");
            break;
        }
    }

    close(sockfd);
}