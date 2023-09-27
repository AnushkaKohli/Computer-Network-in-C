#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct sockaddr_in serv_addr;
int skfd, r, w;
// The port number used by the server.
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
    printf("TCP FILE SEARCH CLIENT\n");

    // Create a TCP socket.
    if ((skfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("CLIENT ERROR: Cannot create socket.\n");
        exit(1);
    }

    // Request a connection to the server.
    if ((connect(skfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
    {
        printf("CLIENT ERROR: Cannot connect to server.\n");
        close(skfd);
        exit(1);
    }
    printf("CLIENT: Connected to server.\nEnter bye to stop.\n");

    for (;;)
    {
        printf("CLIENT: Enter the file name to search: ");
        gets(sbuff);
        // Send a message to the server.
        if ((w = write(skfd, sbuff, 128)) < 0)
        {
            printf("CLIENT ERROR: Cannot send message to the server.\n");
            close(skfd);
            exit(1);
        }

        if (strcmp(sbuff, "bye") == 0)
        {
            printf("CLIENT: Closing connection.\n");
            close(skfd);
            break;
        }

        do
        {
            // Read message from the server.
            r = read(skfd, rbuff, 128);
            if (strcmp(rbuff, "end") == 0)
            {
                printf("SERVER: Message ended after file search\n");
                break;
            }
            else if (r < 0)
            {
                printf("CLIENT ERROR: Cannot read message from the server.\n");
                exit(1);
            }

            else
            {
                rbuff[r] = '\0';
                // Print the received message on console.
                printf("%s", rbuff);
            }
        } while (1);

        // Read file name to return its contents.
        printf("CLIENT: Enter the file name to open: ");
        gets(sbuff);
        // Send a message to the server.
        if ((w = write(skfd, sbuff, 128)) < 0)
        {
            printf("CLIENT ERROR: Cannot send message to the server.\n");
            close(skfd);
            exit(1);
        }

        if (strcmp(sbuff, "bye") == 0)
        {
            printf("CLIENT: Closing connection.\n");
            close(skfd);
            break;
        }

        do
        {
            // Read message from the server.
            r = read(skfd, rbuff, 128);
            if (strcmp(rbuff, "end") == 0)
            {
                printf("SERVER: Message ended after opening file\n");
                break;
            }
            else if (r < 0)
            {
                printf("CLIENT ERROR: Cannot read message from the server.\n");
                exit(1);
            }

            else
            {
                rbuff[r] = '\0';
                // Print the received message on console.
                printf("%s \n", rbuff);
            }
        } while (1);
    }
    close(skfd);
    exit(1);
    return 0;
}