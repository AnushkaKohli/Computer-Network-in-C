/*UDP Chat Client Program*/

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
    struct sockaddr_in serv_addr;
    int skfd, r, s, serv_addr_len;
    int short serv_port = 25021;  /*port number used by the server*/
    char serv_ip[] = "127.0.0.1"; /*server's IP-address*/
    char rbuff[128];              /*buffer for receiving messages*/
    char sbuff[128];              /*buffer for sending messages*/

    /*initializing server socket address structure with zero values*/
    bzero(&serv_addr, sizeof(serv_addr));

    /*filling up the server socket address structure with appropriate values*/
    serv_addr.sin_family = AF_INET;            /*address family*/
    serv_addr.sin_port = htons(serv_port);     /*port number*/
    inet_aton(serv_ip, (&serv_addr.sin_addr)); /*IP-address*/

    printf("\nUDP CHAT CLIENT\n");

    /*creating socket*/
    if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("CLIENT ERROR: Cannot create socket.\n");
        exit(1);
    }
    while (1)
    {
        printf("CLIENT: Enter message to send to server: ");
        gets(sbuff);
        /*sending message to the echo server*/
        if ((s = sendto(skfd, sbuff, 128, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
        {
            printf("CLIENT ERROR: Cannot send message to the server.\n");
            close(skfd);
            exit(1);
        }
        if (strcmp(sbuff, "bye") == 0)
        {
            printf("CLIENT: Closing connection.\n");
            break;
        }

        /*client waits, till server echoes back the message and then receivies it*/
        serv_addr_len = sizeof(serv_addr);
        if ((r = recvfrom(skfd, rbuff, 128, 0, (struct sockaddr *)&serv_addr, &serv_addr_len)) < 0)
        {
            printf("CLIENT ERROR: Cannot receive.\n");
        }
        else
        {
            rbuff[r] = '\0';
            if (strcmp(rbuff, "bye") == 0)
            {
                printf("CLIENT: Server closed connection.\n");
                break;
            }
        }
        printf("CLIENT: Received message from the server: %s\n", rbuff); /*received 'r' bytes from server*/
    }
    close(skfd);
    exit(1);
    return 0;
}
