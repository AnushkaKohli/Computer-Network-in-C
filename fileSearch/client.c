#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main()
{
    struct sockaddr_in serv_addr;

    int skfd, r, s, serv_addr_len;

    int short serv_port = 25021; /*port number used by the server*/

    char serv_ip[] = "127.0.0.1"; /*server's IP-address*/

    char rbuff[128]; /*buffer for receiving messages*/
    char sbuff[128]; /*buffer for sending messages*/

    /*initializing server socket address structure with zero values*/
    bzero(&serv_addr, sizeof(serv_addr));

    /*filling up the server socket address structure with appropriate values*/
    serv_addr.sin_family = AF_INET;            /*address family*/
    serv_addr.sin_port = htons(serv_port);     /*port number*/
    inet_aton(serv_ip, (&serv_addr.sin_addr)); /*IP-address*/

    printf("\nUDP FILE SEARCH CLIENT.\n");

    /*creating socket*/
    if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("\nCLIENT ERROR: Cannot create socket.\n");
        exit(1);
    }

    printf("CLIENT: Connected to server.\nEnter bye to stop.\n");
    while (1)
    {
        printf("CLIENT: Enter the file name to search: ");
        gets(sbuff);

        if ((s = sendto(skfd, sbuff, 128, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
        {
            printf("\nCLIENT ERROR: Cannot send.\n");
            close(skfd);
            exit(1);
        }
        if (strcmp(sbuff, "bye") == 0)
        {
            printf("CLIENT: Closing connection.\n");
            exit(1);
        }
        printf("CLIENT: Message sent to the server: %s\n", sbuff); /*sent 's' bytes to server*/

        serv_addr_len = sizeof(serv_addr);
        do
        {
            r = recvfrom(skfd, rbuff, 128, 0, (struct sockaddr *)&serv_addr, &serv_addr_len);
            if (strcmp(rbuff, "end") == 0)
            {
                printf("SERVER: Message ended after file search.\n");
                break;
            }
            else if (r < 0)
            {
                printf("CLIENT ERROR: Cannot receive.\n");
                close(skfd);
                exit(1);
            }
            else
            {
                rbuff[r] = '\0';
                printf(" %s", rbuff); /*received 'r' bytes from server*/
            }

        } while (1);
        // Read file name to return its contents.
        printf("CLIENT: Enter the file name to open: ");
        gets(sbuff);
        if ((s = sendto(skfd, sbuff, 128, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
        {
            printf("\nCLIENT ERROR: Cannot send.\n");
            close(skfd);
            exit(1);
        }
        if (strcmp(sbuff, "bye") == 0)
        {
            printf("CLIENT: Closing connection.\n");
            exit(1);
        }
        printf("CLIENT: Message sent to the server: %s\n", sbuff); /*sent 's' bytes to server*/

        // serv_addr_len = sizeof(serv_addr);
        do
        {
            r = recvfrom(skfd, rbuff, 128, 0, (struct sockaddr *)&serv_addr, &serv_addr_len);
            if (strcmp(rbuff, "end") == 0)
            {
                printf("SERVER: Message ended after file search.\n");
                break;
            }
            else if (r < 0)
            {
                printf("CLIENT ERROR: Cannot receive.\n");
                close(skfd);
                exit(1);
            }
            else
            {
                rbuff[r] = '\0';
                printf(" %s\n", rbuff); /*received 'r' bytes from server*/
            }

        } while (1);
    }
    close(skfd);
    exit(1);
    return 0;
}
