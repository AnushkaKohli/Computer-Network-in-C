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
/* These arrays are used as buffers to store data received from clients (`rbuff`) and data to be sent to clients (`sbuff`) in the TCP Echo Server program. */
char rbuff[128];
char sbuff[128] = "===Good Morning!===";

int main()
{
    // It is used to set all the bytes in the server socket address to 0.
    bzero(&serv_addr, sizeof(serv_addr));
    // Fill in the server socket's address information.
    serv_addr.sin_family = AF_INET;          // Address family to use.
    serv_addr.sin_port = htons(serv_port);   // Port number.
    inet_aton(serv_ip, &serv_addr.sin_addr); // IP address to listen on.
    printf("TCP Echo Client\n");

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
    printf("CLIENT: Connected to server.\n");

    // Send a message to the echo server.
    if ((w = write(skfd, sbuff, 128)) < 0)
    {
        printf("CLIENT ERROR: Cannot send message to the echo server.\n");
        close(skfd);
        exit(1);
    }

    // Read back the echoed message from the server.
    if ((r = read(skfd, rbuff, 128)) < 0)
        printf("CLIENT ERROR: Cannot read message from the echo server.\n");

    else
    {
        rbuff[r] = '\0';
        // Print the received message on console.
        printf("CLIENT: Received message from the echo server: %s\n", rbuff);
    }
    close(skfd);
    exit(1);
    return 0;
}