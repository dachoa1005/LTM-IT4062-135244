#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>

//fonction pour comparer deux strings : renvoie 0 si les valeurs sont egales et -1 sinon
int compare_strings(char a[], char b[])
{
    int c = 0;
    while (a[c] == b[c]) 
    {
        if (a[c] == '\0' || b[c] == '\0')
        break;
        c++;
    }
    if (a[c] == '\0' && b[c] == '\0')
    return 0;
    else
    return -1;
}

int main() {
    //déclaration des variables
    int clientSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    int cmdEXIT = 0;

    //paramètrage du socket
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    addr_size = sizeof serverAddr;

    //connection au serveur
    connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

    //continuer à envoyer et recevoir des messages 
    //tant qu'un des clients n'envoive pas "exit"
    while (cmdEXIT == 0)
    {
        //la valeur de recv qui est égale a 1 si recv n'a pas 
        //encore reçu de message
        //sinon, elle est egale au nombre de bits reçu
        int recvValue = recv(clientSocket, buffer, sizeof buffer - 1, 0);
        //si recv n'est pas égal a 1 => un message a été reçu
        if (recvValue != 1)
        {
            //si le contenu n'est pas "exit"        
            if (compare_strings(buffer, "exit")==-1)
            {
                //afficher le message du Client1
                printf("Client 1 : ");
                printf("%s\n", buffer);
                memset(&buffer[0], 0, sizeof(buffer));

            }
            //sinon sortir de la boucle
            else cmdEXIT = 1;
        }
        else
        {
            //Client2 peut saisir son message 
            printf("Client 2 : ");
            scanf(" %[^\n]s", buffer);
            //Client2 envoie son message au serveur
            send(clientSocket,buffer,sizeof buffer - 1,0);
            //si le contenu n'est pas "exit"
            if (compare_strings(buffer, "exit")==-1)
            {
                //vider le buffer
                memset(&buffer[0], 0, sizeof(buffer));
            }
            //sinon sortir de la boucle
            else cmdEXIT = 1;
        }   
    }
    return 0;
}