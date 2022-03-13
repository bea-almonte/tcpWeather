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

std::string tcpServer::ConvertoString(char message[]) {
    std::string strMsg(message);
    return strMsg;
}

int tcpServer::LocationExists(std::string locationName) {
    for (unsigned int long i = 0; i < locations.size(); i++) {
        if (locations.at(i).locName == locationName) {
            return i;
        }
    }
    return -1;
}

int tcpServer::FindPos(std::string inputName) {
    for (long unsigned int i = 0; i < users.size(); i++) {
        if (inputName == users.at(i).username) {
            return i;
        }
    }
    return -1;
}

void tcpServer::SubscribeUser(std::string locationName, User tempUser) {
    std::cout << "Location request: " << locationName << std::endl;
    Location tempLocation;
    if (LocationExists(locationName) < 0) {
        tempLocation.locName = locationName;
        locations.push_back(tempLocation);
        locations.at(LocationExists(locationName)).subscribeUser(tempUser.username, tempUser.sock);
    } else {
        locations.at(LocationExists(locationName)).subscribeUser(tempUser.username, tempUser.sock);
    }
    locations.at(LocationExists(locationName)).DisplayUsers();
    tempUser.SuscribeLocation(locationName);
    // send 400

    userMtx.lock();
    users.at(FindPos(tempUser.username)) = tempUser;
    tempUser.SendLocations();
    userMtx.unlock();
    DisplayLocations();
}

void tcpServer::UnsubscribeUser(std::string locationName, User tempUser) {
    std::cout << "Location request: " << locationName << std::endl;
    char server_message[2000];
    Location tempLocation;
    if (LocationExists(locationName) < 0) {
        std::cout << "Location doesn't exist.\n";
        strcpy(server_message,"Loc doesn't exist\n");
        write(tempUser.sock,&server_message,strlen(server_message));
    } else {
        locations.at(LocationExists(locationName)).unsubscribeUser(tempUser.username);
        tempUser.UnuscribeLocation(locationName);
        strcpy(server_message,locationName.c_str());
        write(tempUser.sock,&server_message,strlen(server_message));
    }
    // send 400
    userMtx.lock();
    users.at(FindPos(tempUser.username)) = tempUser;
    tempUser.SendLocations();
    userMtx.unlock();
    DisplayLocations();
    
}

void tcpServer::DisplayLocations() {
    std::cout << "===\nLocs\n";
    for (long unsigned int i = 0; i < locations.size(); i++) {
        std::cout << locations.at(i).locName << ": ";
  /*       for (long unsigned int i = 0; i < locations.at(i).nameSubbed.size(); i++) {
        std::cout << locations.at(i).locName << " ";
        }
        std::cout << std::endl; */
    }
    std::cout <<"\n====\n";
}

void tcpServer::ExecuteCommands(User tempUser) {
    char client_message[2000];//,server_message[2000], 
    int received = 0;
    char code = 'x';
    bool exitUser = false;
    Location tempLocation;
    std::string input;
    while (!exitUser) {
        tempUser = users.at(FindPos(tempUser.username));
        memset(client_message,0,2000);
        received = recv(tempUser.sock, client_message, sizeof(client_message), 0);
        std::string request(client_message);
        if (received <= 0) {
            std::cout << "Error: exiting user\n";
            exitUser = true;
            break;
        }
        code = request[0];
        std::cout << tempUser.username << "'s request: " << code << std::endl;
        
        if (received == 0) {
            break;
        }
        // 200 wait for request
        // 400 sent response
        switch (code) {
            case '0':
                std::cout << "Exiting...\n";
                exitUser = true;
                break;
            case '1':
                std::cout << "Subsrcribe to a location\n";
                // send code 1
                // client sends
                // recv()
                recv(tempUser.sock, client_message, sizeof(client_message), 0);
                input = ConvertoString(client_message);
                SubscribeUser(input,tempUser);

                break;
            case '2':
                std::cout << "Unsub from location\n";
                // send code 1
                // client sends
                recv(tempUser.sock, client_message, sizeof(client_message), 0);
                input = ConvertoString(client_message);

                UnsubscribeUser(input,tempUser);
                // recv()
                // unsub()
                // send 400
                break;
            case '3':
                std::cout << "Display locations subbed\n";
                // send code 
                // client recv()
                // displayloc(locations vec);
                tempUser.SendLocations();
            
                break;
            case '4':
                std::cout << "Send message to location\n";
                break;
            case '5':
                std::cout << "See all online users\n";
                break;
            case '6':
                std::cout << "Send message to user\n";
                break;
            case '7':
                std::cout << "Display last 10 messages\n";
                break;
            case '8':
                std::cout << "Change password\n";
                break;
            default:
                std::cout << "Invalid Request\n";
                break;

        }
    }
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
