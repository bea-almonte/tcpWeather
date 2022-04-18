// bea almonte
// Due: 3/13/21
// tcpServer.cpp
// contains functions to read the requests the clients make
// and send appropriate responses
#include "tcpServer.hpp"
#include <errno.h>

// Sets port, creates and binds socket
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
        return;
    } 

    // bind socket to port
    std::cout << "Binding socket to port.\n\n";
    address.sin_family = AF_INET;
    address.sin_port = htons(portNumber);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(serverSock, (struct sockaddr*)&address,sizeof(struct sockaddr_in)) < 0) {
        std::cerr << "Error: cannot bind socket to port.\n";
        return;
    }

}

// marks socket as passive, start accepting clients
// when a client is accepted, a thread is created for that client
bool tcpServer::AcceptClients() {
    int clientSock = -32;
    struct sockaddr address; // address of client
    unsigned int addr_len;   // length of address
    if (listen(serverSock,20) < 0) {
        std::cerr << "Error: Cannot listen on port.\n";
        return false;
    }

    std::cout << "Server ready and listening.\n";
    while (true) {
        // accept connection
        clientSock = accept(serverSock, &address, &addr_len);

        if (clientSock <= 0) {
            perror("Error is: ");
            std::cout<< "Socket couldn't connect.\n";
            return false;
        }
        //std::cout << "Created thread.\n";
        // creates thread and detaches
        std::thread NewClient(&tcpServer::Process, this, clientSock);
        NewClient.detach();
    }
    return true;
}

// logout user, remove from vector, unsubscribe from locations
void tcpServer::LogoutUser(std::string userDel) {
    // find position of user to be deleted
    User tempUser;
    tempUser = users.at(FindPos(userDel));

    // unsubscribe from location
    for (long unsigned int i = 0; i < locations.size(); i++) {
        locations.at(i).unsubscribeUser(userDel);
    }

    /* for (long unsigned int i = 0; i < locations.size(); i++) {
        locations.at(i).DisplayUsers();
    } */

    // erase from vector
    for (long unsigned int i = 0; i < users.size(); i++) {
        if (userDel == users.at(i).username) {
            users.erase(users.begin() + i);
        }
    }
}

// displays online users
void tcpServer::DisplayOnline(int userSock) {
    std::string usersString;
    char server_message[2000];
    usersString += "User online:\n";

    for (long unsigned int i = 0; i < users.size(); i++) {
        usersString += users.at(i).username;
        usersString += "\n";
    }
    sprintf(server_message,usersString.c_str());
    write(userSock,&server_message,strlen(server_message));
}

// convert char to string
std::string tcpServer::ConvertoString(char message[]) {
    std::string strMsg(message);
    return strMsg;
}

// check if location exists in vector
int tcpServer::LocationExists(std::string locationName) {
    for (unsigned int long i = 0; i < locations.size(); i++) {
        if (locations.at(i).locName == locationName) {
            return i;
        }
    }
    return -1;
}

// Find the position of a user in the vector
// returns -1 if user is not online
int tcpServer::FindPos(std::string inputName) {
    for (long unsigned int i = 0; i < users.size(); i++) {
        if (inputName == users.at(i).username) {
            return i;
        }
    }
    return -1;
}

// subscribe user to a location
// if location is already active, subscribe user to location
// else create location, and subscribe user to location
// update the user vector
void tcpServer::SubscribeUser(std::string locationName, User tempUser) {
    Location tempLocation;
    char server_message[2000];
    locMtx.lock();
    if (LocationExists(locationName) < 0) {
        tempLocation.locName = locationName;
        locations.push_back(tempLocation);
        if (locations.at(LocationExists(locationName)).subscribeUser(tempUser.username, tempUser.sock)) {
            sprintf(server_message,"You are successfully subscribed to %s\n",locationName.c_str());
            write(tempUser.sock,&server_message,strlen(server_message));
        } else {
            sprintf(server_message,"Oh no! You are already subscribed to %s\n",locationName.c_str());
            write(tempUser.sock,&server_message,strlen(server_message));
        }
        
    } else {
        if (locations.at(LocationExists(locationName)).subscribeUser(tempUser.username, tempUser.sock)) {
            sprintf(server_message,"You are successfully subscribed to %s\n",locationName.c_str());
            write(tempUser.sock,&server_message,strlen(server_message));
        } else {
            sprintf(server_message,"Oh no! You are already subscribed to %s\n",locationName.c_str());
            write(tempUser.sock,&server_message,strlen(server_message));
        }
    }
    locMtx.unlock();
    //locations.at(LocationExists(locationName)).DisplayUsers();
    tempUser.SuscribeLocation(locationName);
    // send 400

    userMtx.lock();
    users.at(FindPos(tempUser.username)) = tempUser;
    userMtx.unlock();
}

// unsubscribes user from location
void tcpServer::UnsubscribeUser(std::string locationName, User tempUser) {
    char server_message[2000];
    Location tempLocation;
    locMtx.lock();
    if (LocationExists(locationName) < 0) {
        std::cout << "Location doesn't exist.\n";
        strcpy(server_message,"Location doesn't exist\n");
        write(tempUser.sock,&server_message,strlen(server_message));
    } else {
        locations.at(LocationExists(locationName)).unsubscribeUser(tempUser.username);
        tempUser.UnuscribeLocation(locationName);
        sprintf(server_message,"Removed %s from subscriptions", locationName.c_str());
        write(tempUser.sock,&server_message,strlen(server_message));
    }
    locMtx.unlock();

    // update vector remove location
    userMtx.lock();
    users.at(FindPos(tempUser.username)) = tempUser;
    userMtx.unlock();
    //DisplayLocations();
    
}

void tcpServer::DisplayLocations() {
    for (long unsigned int i = 0; i < locations.size(); i++) {
        std::cout << locations.at(i).locName << ": ";
  /*       for (long unsigned int i = 0; i < locations.at(i).nameSubbed.size(); i++) {
        std::cout << locations.at(i).locName << " ";
        }
        std::cout << std::endl; */
    }
}

void tcpServer::BroadcastMessage(std::string sender, std::string message, int locationPos, int senderSock) {
    char server_message[2000];
    memset(server_message,0,2000);
    std::cout << "Sending1: "  << locations.at(locationPos).totalSubscribed <<"g\n";

    for (int i = 0; i < locations.at(locationPos).totalSubscribed; i++) {
        if (locations.at(locationPos).socketSubbed.at(i) != senderSock) {
            sprintf(server_message,"From %s: %s\n",sender.c_str(), message.c_str());
            std::cout << "Sending: "  << server_message;
            write(locations.at(locationPos).socketSubbed.at(i),&server_message,strlen(server_message));
            // update messages vector
            userMtx.lock();
            users.at(FindPos(locations.at(locationPos).nameSubbed.at(i))).AddMessage(message,sender);
            userMtx.unlock();
        }
     
    }
    
    
}

void tcpServer::SendPrevMsgs(int userPos) {
    char server_message[2000];
    memset(server_message,0,2000);
    userMtx.lock();
    strcpy(server_message,users.at(userPos).MsgToStr().c_str());
    write(users.at(userPos).sock,&server_message,strlen(server_message));
    userMtx.unlock();
}

void tcpServer::PrivateMsg(std::string sender, std::string message, int recSock) {
    char server_message[2000];
    memset(server_message,0,2000);
    sprintf(server_message,"From %s: %s\n",sender.c_str(), message.c_str());
    std::cout << "Sending: "  << server_message;
    write(users.at(recSock).sock,&server_message,strlen(server_message));
    // update messages vector
    userMtx.lock();
    users.at(recSock).AddMessage(message,sender);
    userMtx.unlock();

}

void tcpServer::ExecuteCommands(User tempUser) {
    char client_message[2000],server_message[2000];
    int received = 0;
    char code = 'x';
    bool exitUser = false;
    int LocPos = 0; // location position
    int SendSock = 0;
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
        // only check first character
        code = request[0];
        std::cout << tempUser.username << ">" << code << std::endl;
        
        if (received == 0) {
            break;
        }

        // according to project specficiations
        switch (code) {
            case '0':
                //std::cout << "Exiting...\n";
                exitUser = true;
                break;
            case '1':
                // std::cout << "Subsrcribe to a location\n";
                // send code 1
                // client sends
                // recv()
                recv(tempUser.sock, client_message, sizeof(client_message), 0);
                input = ConvertoString(client_message);
                SubscribeUser(input,tempUser);

                break;
            case '2':
                // std::cout << "Unsub from location\n";
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
                // std::cout << "Display locations subbed\n";
                // send code 
                // client recv()
                // displayloc(locations vec);
                tempUser.SendLocations();
            
                break;
            case '4':
                //std::cout << "Send message to location\n";
                // send locations subbed to
                tempUser.SendLocations();
                memset(server_message,0,2000);
                // receive location to send message to
                recv(tempUser.sock, client_message, sizeof(client_message), 0);
                input = ConvertoString(client_message);
                std::cout << "Attempting to send message to " << input << ".\n";
                LocPos = LocationExists(input);
                // message
                
                memset(client_message,0,2000);
                recv(tempUser.sock, client_message, sizeof(client_message), 0);
                input = ConvertoString(client_message);
                std::cout << "Message: " << input << ".\n";
                if (LocPos >= 0) {
                    locations.at(LocPos).DisplayUsers();
                    // recc
                    BroadcastMessage(tempUser.username, input, LocPos,tempUser.sock);
                } else {
                    strcpy(server_message,"Not subscribed to location.");
                    write(tempUser.sock,&server_message,strlen(server_message));

                }
                break;
            case '5':
                //std::cout << "See all online users\n";
                userMtx.lock();
                DisplayOnline(tempUser.sock);
                userMtx.unlock();
                break;
            case '6':
                //std::cout << "Send message to user\n";
                userMtx.lock();
                DisplayOnline(tempUser.sock);
                userMtx.unlock();

                memset(server_message,0,2000);
                // receive user to send message to
                recv(tempUser.sock, client_message, sizeof(client_message), 0);
                input = ConvertoString(client_message);
                std::cout << "Attempting to send message to " << input << ".\n";
                SendSock = FindPos(input);
                // message
                memset(client_message,0,2000);
                recv(tempUser.sock, client_message, sizeof(client_message), 0);
                input = ConvertoString(client_message);
                if (SendSock >= 0) {
                    // recc
                    PrivateMsg(tempUser.username,input,SendSock);
                } else {
                    strcpy(server_message,"User not online.\n");
                    write(tempUser.sock,&server_message,strlen(server_message));
                }
                break;
            case '7':
                SendPrevMsgs(FindPos(tempUser.username));
                break;
            case '8':
                std::cout << "Change password\n";
                recv(tempUser.sock, client_message, sizeof(client_message), 0);
                input = ConvertoString(client_message);
                ChangePassword(input,tempUser);
                ChangePasswordFile(input, tempUser.username);
                break;
            default:
                std::cout << "Invalid Request\n";
                break;

        }
    }
}

// change password inside user vector
void tcpServer::ChangePassword(std::string newPass, User tempUser) {
    std::string passwords;
    passwords = tempUser.password; // old pass
    tempUser.password = newPass;

    userMtx.lock();
    users.at(FindPos(tempUser.username)) = tempUser;
    userMtx.unlock();
    
    std::cout << users.at(FindPos(tempUser.username)).username << "'s password changed from " << passwords << " to "  << users.at(FindPos(tempUser.username)).password << std::endl;

}

void tcpServer::ChangePasswordFile(std::string newPass, std::string findUser) {
    int posFound = 0;
    // rename password to passwordDel
    std::ifstream inPass;
    std::ofstream outPass;
    std::string temp;
    // rename
    char old[] = "passwords.txt";
    char newName[] = "passwordDel.txt";
    rename(old,newName);
    // findposition
    inPass.open("usernames.txt");

    while (inPass >> temp) {
        if (findUser == temp) {
            break;
        }
        posFound++;
    }

    inPass.close();
    // check findUser in usernames set to Posfound
    // when found then send until posFound
    inPass.open("passwordDel.txt");
    outPass.open("passwords.txt");
    for (int i = 0; i < posFound; i++) {
        inPass >> temp;
        outPass << temp << std::endl;
    }
    inPass >> temp;
    outPass << newPass <<std::endl;

    while (inPass >> temp) {
        outPass << temp << std::endl;
    }
    std::cout << "\'passwords.txt\' updated.\n";
    remove( "passwordDel.txt" );
    inPass.close();
    outPass.close();
}

// checks user vector if user is already logged in
bool tcpServer::IsLoggedIn(std::string newUser) {
    userMtx.lock();
    for (long unsigned int i = 0; i < users.size(); i++) {
        if (newUser == users.at(i).username) {
            return true;
        }
    }
    userMtx.unlock();
    return false;
}

// A client thread
void tcpServer::Process(int sock) {
    User tempUser;
    //char server_message[2000];
    tempUser.sock = sock;
    //bool loggedIn = false;

    std::cout << "Client connected to " << tempUser.sock << std::endl;

    // Get username and password
    // login or register
    while(!tempUser.Login());
    std::cout << "\'" << tempUser.username << "\' succesfully logged in.\n";

    // adds user if logged in
    // exit if client wants to exit
    if (tempUser.exitUser) {
        close(tempUser.sock);
        //users.erase(users.begin() + it);
        std::cout << "Connection with \'" << tempUser.username<<  "\' terminated during login.\n";
        return;
    }

    userMtx.lock();
    users.push_back(tempUser);
    userMtx.unlock();

    // weather app functions
    ExecuteCommands(tempUser);
  
    // log out and delete from vector
    userMtx.lock();
    LogoutUser(tempUser.username); // update to remove subscriptions
    userMtx.unlock();

    close(tempUser.sock);

    std::cout << "Connection with \'" << tempUser.username << "\' terminated\n";

/*     userMtx.lock();
    DisplayOnline();
    userMtx.unlock(); */
}
