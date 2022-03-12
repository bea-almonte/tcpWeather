#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring> // memcpy
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>

int main(int argc, char ** argv)
{
    int port = 60000;
    int sock = -1;
    struct sockaddr_in address;
    struct hostent * host;
    char server_message[2000], client_message[2000];

        /* checking commandline parameter */
/*     if (argc != 4)
    {
        printf("usage: %s hostname port text\n", argv[0]);
        return -1;
    } */

        /* obtain port number */
/*     if (sscanf(argv[2], "%d", &port) <= 0)
    {
        fprintf(stderr, "%s: error: wrong parameter: port\n", argv[0]);
        return -2;
    }
 */
        /* create socket */
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock <= 0)
    {
        fprintf(stderr, "%s: error: cannot create socket\n", argv[0]);
        return -3;
    }

        /* connect to server */
    address.sin_family = AF_INET;
    address.sin_port = htons(60000);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");

/*     if (!host)
    {
        fprintf(stderr, "%s: error: unknown host %s\n", argv[0], argv[1]);
        return -4;
    }
    memcpy(&address.sin_addr, host->h_addr_list[0], host->h_length);
 */
    if (connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("Unable to connect\n");
        return -5;
    }
/*     if (connect(sock, (struct sockaddr*)&address, sizeof(address)) < 0){
        printf("Unable to connect\n");
        return -1;
    }
 */
      /* send text to server */
      int sent = -1;
    while (sent != 0) {
        recv(sock, server_message, sizeof(server_message), 0);
        printf("Server's response: %s\n",server_message);
        std::cout << "\nEnter msg: ";
        std::cin >> client_message;
        write(sock, &client_message, sizeof(client_message));
    }
    

        /* close socket */
    close(sock);

    return 0;
}