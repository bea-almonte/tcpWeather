// bea almonte
// Due: 3/13/21
// description of the class that is used to store locations,
// users, and subscribers
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
    std::mutex locMtx;
    std::vector<User> users;
    std::vector<Location> locations;
    int serverSock;
    int totalUsers;
    void Process(int sock);
    void CreateSocket(int portNumber); // Sets port, creates and binds socket
    bool AcceptClients(); // marks socket as passive, start accepting clients
    private:
    void LogoutUser(std::string userDel); // logouts, unsubs
    void DisplayOnline(int userSock); // displays sockets and online users
    void ExecuteCommands(User tempUser); // main request loop, executes command based on request 
    std::string ConvertoString(char message[]); // convert char to string
    int LocationExists(std::string locationName); // check if location exists in vector
    void SubscribeUser(std::string locationName, User tempUser); // subscribe use to location
    void UnsubscribeUser(std::string locationName, User tempUser); // unsubscribe user to location
    void DisplayLocations();
    int FindPos(std::string inputName); // find position of user given username
    void ChangePassword(std::string newPass, User tempUser);
    void ChangePasswordFile(std::string newPass, std::string findUser);
    bool IsLoggedIn(std::string newUser); // check if user is logged in
    void BroadcastMessage(std::string sender, std::string message, int locationPos, int senderSock); // send msg to location
    void PrivateMsg(std::string sender, std::string message, int recSock); // send private msg from sender to user
    void SendPrevMsgs(int userPos); // sends 10 recent messages to client
};

#endif