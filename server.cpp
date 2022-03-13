#include <iostream>
#include "tcpServer.hpp"



/* void * Process(void * ptr);
int CreateSocket(int portNumber);

typedef struct {
    int sock; // socket of client
    struct sockaddr address; // address of client
    unsigned int addr_len;   // length of address
} connection_t; */

int main (int argc, char** argv) {
    tcpServer Weather;

    if (argc > 1)
    {
        Weather.CreateSocket(std::stoi(argv[1]));
    } else {
        Weather.CreateSocket(60022);
    }   

    Weather.AcceptClients();

    return 0;
}