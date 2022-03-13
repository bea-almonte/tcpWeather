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
#include <sstream> 
#include <arpa/inet.h> // inet_addr
#include <vector>

#ifndef USER_HPP
#define USER_HPP

class User {
    public:
        std::string username;
        std::string password;
        std::vector<std::string> locations;
        int filePos; // user's position on username.txt and file.txt
        int sock; // socket
        struct sockaddr address; // address of client
        unsigned int addr_len;   // length of address
        // functions
        void SetUsername(std::string userInput);
        bool Login();
        int CheckUsername(std::string userInput); // check if input is in file
        bool CheckPassword(int userPos, std::string inputPass);
        void RegisterUser(std::string userInput, std::string userPass);
        void SendLocations();
        void SendMessage(std::string location);
        void SuscribeLocation(std::string location);
        void UnuscribeLocation(std::string location);
        void WaitRecv(int socket);
        void ChooseFunc();
        bool UniqueUser();
        bool exitUser;
};

#endif