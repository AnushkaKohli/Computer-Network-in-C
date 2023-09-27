#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* The line `struct sockaddr_in serv_addr, cli_addr;` is declaring two variables of type `struct sockaddr_in`. */
struct sockaddr_in serv_addr, cli_addr;
int listenfd, connfd, r, w, val, cli_addr_len;
// The port number that the server will listen on.
unsigned short serv_port = 25020;
// Server IP address.
char serv_ip[] = "127.0.0.1";
// Buffer used to store data received from and sent to clients.
char rbuff[128]; // buffer for receiving
char sbuff[128]; // buffer for sending

int main()
{
    // It is used to set all the bytes in the server socket address to 0.
    bzero(&serv_addr, sizeof(serv_addr));
    // Fill in the server socket's address information.
    serv_addr.sin_family = AF_INET;          // Address family to use.
    serv_addr.sin_port = htons(serv_port);   // Port number.
    inet_aton(serv_ip, &serv_addr.sin_addr); // IP address to listen on.
    printf("\nTCP CHAT SERVER\n");

    // Create a TCP socket.
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("SERVER ERROR: Cannot create socket.\n");
        exit(1);
    }

    // Bind the server socket address to the specified address.
    if ((bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
    {
        printf("SERVER ERROR: Cannot bind.\n");
        close(listenfd);
        exit(1);
    }

    // Listen for client connection requests.
    if ((listen(listenfd, 5)) < 0)
    {
        printf("SERVER ERROR: Cannot listen.\n");
        close(listenfd);
        exit(1);
    }
    cli_addr_len = sizeof(cli_addr);

    while (1)
    {
        printf("SERVER: Listening for client connections.\n");
        if ((connfd = accept(listenfd, (struct sockaddr *)&cli_addr, &cli_addr_len)) < 0)
        {
            printf("SERVER ERROR: Cannot accept client connections.\n");
            close(listenfd);
            exit(1);
        }
        printf("SERVER: Connection from client %s accepted.\n", inet_ntoa(cli_addr.sin_addr));
        printf("SERVER: Press Ctrl + C to stop the server.\nEnter bye to stop the chat.\n");
        for (;;)
        {
            if ((r = read(connfd, rbuff, 128)) < 0)
                printf("SERVER ERROR: Cannot receive message from client\n");
            else
            {
                rbuff[r] = '\0';
                printf("SERVER: Received from client: %s\n", rbuff);
                if (strcmp(rbuff, "bye") == 0)
                {
                    printf("SERVER: Client %s has closed the connection.\n", inet_ntoa(cli_addr.sin_addr));
                    close(connfd);
                    break;
                }
                printf("SERVER: Enter message to send to client: ");
                gets(sbuff);

                // Send message to client.
                if ((w = write(connfd, sbuff, 128)) < 0)
                    printf("SERVER ERROR: Cannot send message to the client.\n");
                else
                    printf("SERVER: Message sent to client: %s\n", sbuff);
                if (strcmp(sbuff, "bye") == 0)
                {
                    printf("SERVER: Closing connection with client %s.\n", inet_ntoa(cli_addr.sin_addr));
                    close(connfd);
                    break;
                }
            }
        }
    }
    return 0;
}