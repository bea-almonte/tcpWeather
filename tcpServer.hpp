#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
//#include <pthread.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <arpa/inet.h> // inet_addr
#include <vector>
#include <thread> // thread()
#include <mutex> // lock(), unlock()
#include "user.hpp"
#include "location.hpp"

#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

class tcpServer {
    public:
    std::mutex userMtx;
    std::vector<User> users;
    std::vector<Location> locations;
    int serverSock;
    int totalUsers;
    void Process(int sock);
    void CreateSocket(int portNumber);
    void AcceptClients();
    void LogoutUser(std::string userDel);
    void DisplayOnline();
    void ExecuteCommands(User tempUser);
    std::string ConvertoString(char message[]);
    int LocationExists(std::string locationName);

};

#endif