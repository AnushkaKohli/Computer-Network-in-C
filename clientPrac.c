#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

struct sockaddr_in serv_addr;
int skfd, r, w;
unsigned short serv_port = 35020;
char serv_ip[] = "127.0.0.1";
char rbuff[128], sbuff[128];

int main()
{
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_port = htons(serv_port);
    serv_addr.sin_family = AF_INET;
    inet_aton(serv_ip, (&serv_addr.sin_addr));
    printf("\nTCP BINARY CAL CLIENT\n");

    if ((skfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("CLIENT ERROR: Cannot create socket.\n");
        exit(1);
    }

    if ((connect(skfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
    {
        printf("SERVER ERROR: Cannot bind.\n");
        close(skfd);
        exit(1);
    }

    printf("CLIENT: Successfully connected to the server %s.\n", inet_ntoa(serv_addr.sin_addr));

    while (1)
    {
        printf("CLIENT: Enter the expression: ");
        gets(sbuff);

        if ((w = write(skfd, sbuff, 128)) < 0)
        {
            printf("CLIENT ERROR: Cannot send message to client.\n");
            close(skfd);
            exit(1);
        }
        printf("CLIENT: Message successfully sent to client.\n");
        if ((strcmp(sbuff, "bye")) == 0)
        {
            printf(("CLIENT: Client sent bye. Closing connection.\n"));
            close(skfd);
            break;
        }
        do
        {
            if ((r = read(skfd, rbuff, 128)) < 0)
            {
                printf("CLIENT ERROR: Cannot receive message from client.\n");
                close(skfd);
                exit(1);
            }
            if (strcmp(rbuff, "end") == 0)
            {
                printf("CLIENT: Message ended.\n");
                break;
            }
            else
            {
                rbuff[r] = '\0';
                printf("CLIENT: The result of the expression is: %s\n", rbuff);
            }
        } while (1);
    }

    close(skfd);
    exit(1);
    return 0;
}