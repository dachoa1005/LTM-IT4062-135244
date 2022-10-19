#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char const *argv[])
{
    char parameter[100];
    int hostname; // host name = 1 mean user input a host name, host name = 0 mean user input an ip address
    char hostbuffer[100];

    // get parameter
    if (argc == 2)
    {
        strcpy(parameter, argv[1]);
        printf("The parameter is: %s \n", parameter);
    }
    else
    {
        printf("Usage: ./resolver <domain name>");
    }

    // define that parameter is a domain name or ip address
    if (isalpha(parameter[0]))
    {
        printf("The parameter is a domain name \n");
        hostname = 1;
    }
    else
    {
        printf("The parameter is an ip address \n");
        hostname = 0;
    }

    struct hostent *host_entry;

    // get IP address from domain name
    if (hostname == 1)
    {
        struct hostent *host_entry = gethostbyname(parameter);
        if (host_entry == NULL)
        {
            printf("Error: %s\n", hstrerror(h_errno));
        }
        else
        {
            printf("Host name: %s\n", host_entry->h_name);
            printf("Official IP: %s\n", inet_ntoa(*(struct in_addr *)host_entry->h_addr_list[0]));
            printf("All IP addresses: \n");
            int i = 0;
            while (host_entry->h_addr_list[i] != NULL)
            {
                printf("%s\n", inet_ntoa(*(struct in_addr *)host_entry->h_addr_list[i]));
                i++;
            }
        }
    }

    // get domain name from IP address
    struct in_addr **addr_list;
    struct hostent *host = gethostbyaddr(host_entry->h_addr_list[0], sizeof(host_entry->h_addr_list[0]), AF_INET);
    if (host == NULL)
    {
        printf("Error: %s\n", hstrerror(h_errno));
    }
    else
    {
        printf("Host name: %s\n", host->h_name);
        printf("IP address: %s\n", inet_ntoa(*(struct in_addr *)host->h_addr_list[0]));
    }
    return 0;
}
