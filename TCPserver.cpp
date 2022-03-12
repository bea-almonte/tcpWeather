// Bea Almonte
// httpServer.cpp
// This code creates an HTTP server capable of parsing requests
// and sending files

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <arpa/inet.h> // inet_addr
#include <vector>
#include "user.hpp"
//#include "location.hpp"
//#include <thread>

std::vector<User> users;
//std::vector<Location> locations;
int totalUsers = 0;

void * Process(void * ptr);
int CreateSocket(int portNumber);

int main (int argc, char** argv) {
    pthread_t thread;
    int serverSocket = 0;
    int portNumber = 0;
    User tempUser;

    if (argc > 1)
    {
        portNumber = std::stoi(argv[1]);
    }
    serverSocket = CreateSocket(portNumber);

    if (listen(serverSocket,5) < 0) {
        std::cerr << "Error: Cannot listen on port.\n";
        return -3;
    }

    std::cout << "Server ready and listening.\n";

    while (true) {
        // accept connection
        //connection = (connection_t *)malloc(sizeof(connection_t));
        
        tempUser.sock = accept(serverSocket, &tempUser.address, &tempUser.addr_len);
        users.push_back(tempUser);
        std::cout << "\n===============================\n";
        std::cout << "Created thread.\n";
        totalUsers = users.size() -1;
        pthread_create(&thread,0,Process, &totalUsers);
        pthread_detach(thread);
    
    }
    return 0;
}

int CreateSocket(int portNumber) {
    int sock = -1;
    struct sockaddr_in address;

        
    if (portNumber < 60001 || portNumber > 60099) {
            portNumber = 60001;
            std::cout << "Invalid port input, setting to default port:" << portNumber << std::endl << std::endl;
    } else {
            std::cout << "Setting port to: " << portNumber << std::endl << std::endl;
    }
    
    // create socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock <= 0) {
        std::cerr << "Error cannot create socket.\n";
        return -1;
    } 

    // bind socket to port
    std::cout << "Binding socket to port.\n\n";
    address.sin_family = AF_INET;
    address.sin_port = htons(portNumber);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sock, (struct sockaddr*)&address,sizeof(struct sockaddr_in)) < 0) {
        std::cerr << "Error: cannot bind socket to port.\n";
        return -2;
    }

    return sock;
}

void * Process(void * ptr) {
    char * buffer = new char[4096];
    char clientStr[2000], serverStr[2000];
    int it = 0; // iterator
    int *it_ptr;
    bool exitClient = false;
    it_ptr = (int*)ptr;
    it = *it_ptr;


    std::cout << "User #" << it << std::endl;
    // clear buffer
    std::cout << "Sock: " << users.at(it).sock << std::endl;
    memset(buffer,0,4096);

    while (!exitClient) {
        memset(clientStr,0,2000);
        sprintf(serverStr, "Send me messages, client.");
        write(users.at(it).sock, serverStr, strlen(serverStr));
        recv(users.at(it).sock, clientStr, sizeof(clientStr), 0);
        // ss << buffer
        // ss >> request
        // login or register
        std::cout << "User #" << it << std::endl;
        printf("Msg from client: %s\n", clientStr);   
        std::cout << "===============================\n";
    }
    // set username

    // set password
    
    //delete[] buffer;
    
    close(users.at(it).sock);
    users.erase(users.begin() + it);
    pthread_exit(0);
}
