#include "tcpServer.hpp"
#include <errno.h>
void tcpServer::CreateSocket(int portNumber) {
    struct sockaddr_in address;

    if (portNumber < 60001 || portNumber > 60099) {
            portNumber = 60001;
            std::cout << "Invalid port input, setting to default port:" << portNumber << std::endl << std::endl;
    } else {
            std::cout << "Setting port to: " << portNumber << std::endl << std::endl;
    }
    
    // create socket
    serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSock <= 0) {
        std::cerr << "Error cannot create socket.\n";
    } 

    // bind socket to port
    std::cout << "Binding socket to port.\n\n";
    address.sin_family = AF_INET;
    address.sin_port = htons(portNumber);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(serverSock, (struct sockaddr*)&address,sizeof(struct sockaddr_in)) < 0) {
        std::cerr << "Error: cannot bind socket to port.\n";
    }



}

void tcpServer::AcceptClients() {
    int clientSock = -32;
    struct sockaddr address; // address of client
    unsigned int addr_len;   // length of address
    if (listen(serverSock,5) < 0) {
        std::cerr << "Error: Cannot listen on port.\n";
    }

    std::cout << "Server ready and listening.\n";
    while (true) {
        // accept connection
        clientSock = accept(serverSock, &address, &addr_len);
        std::cout << "servSock: " << serverSock << std::endl;
        std::cout << "userSock: " << clientSock << std::endl;
        if (clientSock <= 0) {
            perror("Error is: ");
            std::cout<< "Socket couldn't connect.\n";
            continue;
        }
        std::cout << "Sock b4 thread: " << clientSock << std::endl;
        std::cout << "\n===============================\n";
        std::cout << "Created thread.\n";
        
        std::thread NewClient(&tcpServer::Process, this, clientSock);
        NewClient.detach();
    }
}

void tcpServer::LogoutUser(std::string userDel) {
    for (long unsigned int i = 0; i < users.size(); i++) {
        if (userDel == users.at(i).username) {
            users.erase(users.begin() + i);
        }
    }
}

void tcpServer::DisplayOnline() {
    for (long unsigned int i = 0; i < users.size(); i++) {
        std::cout << "User: " << users.at(i).username << std::endl;
    }
}

void tcpServer::ExecuteCommands(User tempUser) {
    tempUser.WaitRecv(tempUser.sock);
}

void tcpServer::Process(int sock) {
    User tempUser;
    tempUser.sock = sock;
    // clear buffer

    std::cout << "Sock: " << tempUser.sock << std::endl;

    //memset(buffer,0,4096);

    // Get username and password
    // login or register
    while(!tempUser.Login());

    // adds user if logged in
    // exit if client wants to exit
    
    if (tempUser.exitUser){
        close(tempUser.sock);
        //users.erase(users.begin() + it);
        std::cout << "Connection Terminated In Login.\n";
        return;
    }


    userMtx.lock();
    users.push_back(tempUser);
    std::cout <<"1\n";
    DisplayOnline();
    userMtx.unlock();
    

    ExecuteCommands(tempUser);
  
    userMtx.lock();
    LogoutUser(tempUser.username);
    userMtx.unlock();

    close(tempUser.sock);

    std::cout << "Connection Terminated.\n";

    userMtx.lock();
    DisplayOnline();
    userMtx.unlock();
}
