// bea almonte
// Due: 4/17/22
// server.cpp
// runs the tcpServer object

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

    if (!Weather.AcceptClients()) {
        return -1;
    }

    return 0;
}