/*UDP Chat Server Program*/

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    struct sockaddr_in serv_addr, cli_addr;

    int skfd, r, s, cli_addr_len;

    int short serv_port = 25021; /*port number to be used by the server*/

    char serv_ip[] = "127.0.0.1"; /*server's IP-address*/

    char rbuff[128]; // buffer for receiving
    char sbuff[128]; // buffer for sending

    /*initializing server socket address structure with zero values*/
    bzero(&serv_addr, sizeof(serv_addr));

    /*filling up the server socket address structure with appropriate values*/
    serv_addr.sin_family = AF_INET;            /*address family*/
    serv_addr.sin_port = htons(serv_port);     /*port number*/
    inet_aton(serv_ip, (&serv_addr.sin_addr)); /*IP-address*/

    printf("\nUDP CHAT SERVER\n");

    /*creating socket*/
    if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("\nSERVER ERROR: Cannot create socket.\n");
        exit(1);
    }

    /*binding server socket address structure*/
    if ((bind(skfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
    {
        printf("\nSERVER ERROR: Cannot bind.\n");
        close(skfd);
        exit(1);
    }

    for (;;)
    {
        /*server waits, till client sends any  message to be receivied*/
        printf("\nSERVER: Waiting for client...Press Cntrl + C to stop the chat server.\n");
        cli_addr_len = sizeof(cli_addr);
        while (1)
        {
            if ((r = recvfrom(skfd, rbuff, 128, 0, (struct sockaddr *)&cli_addr, &cli_addr_len)) < 0)
            {
                printf("\nSERVER ERROR: Cannot receive.\n");
                close(skfd);
                exit(1);
            }
            else
            {
                rbuff[r] = '\0';
                printf("\nSERVER: Message received from client %s: %s.\n", inet_ntoa(cli_addr.sin_addr), rbuff); /*received 'r' bytes form client*/
                if (strcmp(rbuff, "bye") == 0)
                {
                    printf("SERVER: Client %s has closed the connection.\n", inet_ntoa(cli_addr.sin_addr));
                    close(skfd);
                    break;
                }
                printf("SERVER: Enter message to send to client: ");
                gets(sbuff);
                /*sending, i.e. echoing the same r bytes back to client*/
                if ((s = sendto(skfd, sbuff, 128, 0, (struct sockaddr *)&cli_addr, cli_addr_len)) < 0)
                {
                    printf("\nSERVER ERROR: Cannot send message to the client.\n");
                    // close(skfd);
                    // exit(1);
                }
                // else
                //     printf("SERVER: Message sent to client %s: %s\n", inet_ntoa(cli_addr.sin_addr), sbuff); /*sent 's' bytes to client*/
                if (strcmp(sbuff, "bye") == 0)
                {
                    printf("SERVER: Closing connection with client %s.\n", inet_ntoa(cli_addr.sin_addr));
                    close(skfd);
                    break;
                }
            }
        }
    }
    return 0;
}