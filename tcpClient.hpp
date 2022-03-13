#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring> // memcpy
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>
#include <thread>

class tcpClient {
    public:
    std::string username;
    bool endProgram;
    int port;
    int sock;
    char server_message[2000];
    char client_message[2000];
    struct sockaddr_in address;
    void SetPort(int userPort);
    void ConnectToServer();
    void Login();
    void LoginOutput();
    void SendInput();
    void CloseSocket();
    void OutputMenu();
    void Receive(); // takes messages from server and outputs them
    tcpClient();
};