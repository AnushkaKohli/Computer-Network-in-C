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
int listenfd, connfd, r, w, cli_addr_len;
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
    printf("\nTCP BASIC CALCULATOR SERVER\n");

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
        printf("SERVER: Press Ctrl + C to stop the server.\nEnter bye to stop the server.\n");
        for (;;)
        {
            if ((r = read(connfd, rbuff, 128)) < 0)
                printf("SERVER ERROR: Cannot receive message from client\n");
            else
            {
                rbuff[r] = '\0';
                if (strcmp(rbuff, "bye") == 0) // If the client sends bye, then the server will also exit.
                {
                    printf("SERVER: Client has sent bye. Server will exit now.\n");
                    close(connfd);
                    break;
                }
                printf("SERVER: Expression sent by client is: %s\n", rbuff);
                int k = rbuff[0] - '0';
                char ch = rbuff[1];
                int m = rbuff[2] - '0';
                int result, flag = 0;
                switch (ch)
                {
                case '+':
                    result = k + m;
                    break;
                case '-':
                    result = k - m;
                    break;
                case '*':
                    result = k * m;
                    break;
                case '/':
                    if (m == 0)
                    {
                        strcpy(sbuff, "Division by zero is not possible");
                        flag = 1;
                        break;
                    }
                    else
                    {
                        result = k / m;
                        break;
                    }
                default:
                    strcpy(sbuff, "Invalid Expression");
                    break;
                }
                if (flag == 0)
                    sprintf(sbuff, "%d", result);
                // Send message to client.
                if ((w = write(connfd, sbuff, 128)) < 0)
                    printf("SERVER ERROR: Cannot send message to the client.\n");
                else
                    printf("SERVER: Result sent to client: %s\n", sbuff);
            }
        }
    }
    return 0;
}