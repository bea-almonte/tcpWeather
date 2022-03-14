#include <iostream>
#include "tcpServer.hpp"

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