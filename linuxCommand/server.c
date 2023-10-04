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

    printf("\nUDP LINUX COMMAND SERVER.\n");

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
        printf("\nSERVER: Waiting for client...Press Cntrl + c to stop the server.\n");
        cli_addr_len = sizeof(cli_addr);
        while (1)
        {
            if ((r = recvfrom(skfd, rbuff, 128, 0, (struct sockaddr *)&cli_addr, &cli_addr_len)) < 0)
            {
                printf("\nSERVER ERROR: Cannot receive.\n");
                close(skfd);
                exit(1);
            }
            else if (strcmp(rbuff, "bye") == 0)
            {
                printf("SERVER: Client has sent bye. Server will exit now.\n");
                close(skfd);
                break;
            }
            else
            {
                rbuff[r] = '\0';
                printf("SERVER: Command sent by client is: %s\n", rbuff);

                char result[128];
                FILE *fp;
                sprintf(result, "%s > fp", rbuff);
                system(result);
                fp = fopen("fp", "r");
                if (fp == NULL)
                {
                    printf("SERVER ERROR: Cannot execute command.\n");
                    exit(1);
                }
                // Read the output line by line until the end of the file is reached
                // sbuff[0] = '\0'; // Clear the sbuff
                while (fgets(sbuff, 128, fp) != 0)
                {
                    // printf("%s", sbuff);
                    s = sendto(skfd, sbuff, 128, 0, (struct sockaddr *)&cli_addr, cli_addr_len);
                }
                fclose(fp);
                /*sending message to client*/
                if ((s = sendto(skfd, "end", 128, 0, (struct sockaddr *)&cli_addr, cli_addr_len)) < 0)
                {
                    printf("\nSERVER ERROR: Cannot send.\n");
                    close(skfd);
                    exit(1);
                }
                else
                    printf("\nSERVER: Result of the command sent to client %s.\n", inet_ntoa(cli_addr.sin_addr)); /*sent 's' bytes to client*/
            }
        }
    }
    return 0;
}