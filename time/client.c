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

    char rbuff[128];                              /*buffer for receiving messages*/
    char sbuff[128] = "Please send me the time."; /*buffer for sending messages*/

    /*initializing server socket address structure with zero values*/
    bzero(&serv_addr, sizeof(serv_addr));

    /*filling up the server socket address structure with appropriate values*/
    serv_addr.sin_family = AF_INET;            /*address family*/
    serv_addr.sin_port = htons(serv_port);     /*port number*/
    inet_aton(serv_ip, (&serv_addr.sin_addr)); /*IP-address*/

    printf("\nUDP TIME CLIENT.\n");

    /*creating socket*/
    if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("\nCLIENT ERROR: Cannot create socket.\n");
        exit(1);
    }

    /*sending message to the echo server*/
    if ((s = sendto(skfd, sbuff, 128, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
    {
        printf("\nCLIENT ERROR: Cannot send.\n");
        close(skfd);
        exit(1);
    }
    printf("\nCLIENT: Message sent to the server: %s\n", sbuff); /*sent 's' bytes to server*/

    /*client waits, till server echoes back the message and then receivies it*/
    serv_addr_len = sizeof(serv_addr);
    char result[128];
    if ((r = recvfrom(skfd, rbuff, 128, 0, (struct sockaddr *)&serv_addr, &serv_addr_len)) < 0)
    {
        printf("\nCLIENT ERROR: Cannot receive.\n");
        close(skfd);
        exit(1);
    }
    else
    {
        rbuff[r] = '\0';
        time_t t = time(NULL);
        int sum = 0, i;
        strcpy(result, rbuff);
        for (i = 11; i < 13; i++)
        {
            int temp = result[i] - '0';
            sum = sum * 10 + temp;
        }
        char greeting[128];
        if (sum >= 0 && sum < 12)
            strcpy(greeting, "Good morning!");
        else if (sum >= 12 && sum < 18)
            strcpy(greeting, "Good afternoon!");
        else if (sum >= 18 && sum < 24)
            strcpy(greeting, "Good evening!");
        else
            strcpy(greeting, "Good night!");

        printf("CLIENT: The time of the server is: %s\n", rbuff); /*received 'r' bytes from server*/
        printf("CLIENT: %s\n", greeting);
    }
    close(skfd);
    exit(1);
    return 0;
}
