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
    void SetPort(int userPort);
    void ConnectToServer();
    void Login();
    void LoginOutput();
    void SendInput();
    void CloseSocket();
    void OutputMenu();
    void ReceiveMsg(); // takes messages from server and outputs them
    tcpClient();
};