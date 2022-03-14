// tcpClient.hpp
// implements the client that connects to the server

#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring> // memcpy
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>
#include <thread>
#include <mutex>

class tcpClient {
    public:
        std::string username;
        bool endProgram;
        int port;
        int sock;
        char server_message[2000];
        char client_message[2000];
        std::mutex sockMtx;
        // functions
        tcpClient();
        void SetPort(int userPort);
        bool ConnectToServer();
        void Login();
        void SendInput();
        void CloseSocket();
    private:
        void LoginOutput();
        void OutputMenu();
        void ReceiveMsg(); // takes messages from server and outputs them

};