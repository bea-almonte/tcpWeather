// bea almonte
// Due: 3/13/21
// user.hpp
// implements the user class which contains the login
// and messages (to be implemented in part 2)
#include "user.hpp"

User& User::operator=(const User& createdUser) {
    if (this != &createdUser) {
        this->username = createdUser.username;
        this->password = createdUser.password;
        this->locations.resize(createdUser.locations.size());
        for (long unsigned int i = 0;i < createdUser.locations.size(); i++) {
            this->locations.at(i) = createdUser.locations.at(i);
        }
        this->messages = createdUser.messages;

        this->sock = createdUser.sock;
        this->exitUser = createdUser.exitUser;
    }
    return *this;
}

void User::AddMessage(std::string message, std::string sender) {
    std::string insertedMsg;
    insertedMsg += sender;
    insertedMsg += " - ";
    insertedMsg += message;
    
    if (insertedMsg.size() == 10) {
        messages.pop_back();
    }
    messages.push_front(insertedMsg);
}

std::string User::MsgToStr() {
    std::string finalMsg;
    finalMsg += "Last 10 messages\n";
    for (unsigned long int i = 0; i < messages.size(); i++) {
        finalMsg += messages.at(i);
        finalMsg += "\n";
    }
    return finalMsg;
}

bool User::Login() {
    int bytesRec, code, pos;
    char server_message[2000], client_message[2000];
    exitUser = false;
    std::string inputName, inputPass;

    // clear buffer
    memset(client_message,0,2000);
    memset(server_message,0,2000);

    bytesRec = recv(sock, client_message, sizeof(client_message), 0);
    if (bytesRec <= 0) {
        std::cout << "Client Disconnected.\n";
        exitUser = true;
        return true;
    }
    std::string wholeThing(client_message);
   

    /************************************************************/
    // PARSE INPUT -- fix this 
    pos = wholeThing.find(" ");
    code = std::stoi(wholeThing.substr(0,pos));
    if (code == 3) {
        exitUser = true;
        return true;
    }
    wholeThing.erase(0, pos +1);
    // get username
    pos = wholeThing.find(" ");
    inputName = wholeThing.substr(0,pos);
    wholeThing.erase(0, pos +1);
    // get password input
    inputPass = wholeThing;
    /************************************************************/
    
    // Testing
    /* std::cout << "Code: " << code << std::endl;
    std::cout << inputName << " | " << inputPass << std::endl; */

    // Set username and pass
    username = inputName;
    password = inputPass;


    // 100 succesful login
    // 101 right username, wrong pass
    // 102 user does not exist
    // 103 user already logged in
    if (code == 1) {
        // check username
        if (CheckUsername(username) >= 0) {
            if (CheckPassword(CheckUsername(username),password)) {
                sprintf(server_message,"100");
                write(sock, &server_message, strlen(server_message));
                return true;
            } else {
                sprintf(server_message,"102");
                write(sock, &server_message, strlen(server_message));
                return false;
            }
            
        } else {
            sprintf(server_message,"101");
            write(sock, &server_message, strlen(server_message));
            return false;
        }
        // check password
        
        
    } else if (code == 2) {
        // register
        if (RegisterUser(username, password)) {
            sprintf(server_message,"200");
            write(sock, &server_message, strlen(server_message));
        } else {
            sprintf(server_message,"201");
            write(sock, &server_message, strlen(server_message));
        }
        return false;
    } else {
        // Bad request
        sprintf(server_message,"203");
        write(sock, &server_message, strlen(server_message));
    }

    return false;

}

void User::SetUsername(std::string userInput) {
    username = userInput;
}

// output position of username in file
int User::CheckUsername(std::string userInput) {
    std::ifstream inUsers;
    std::string fileUserName;
    int matchPos = 0;
    inUsers.open("usernames.txt");
    while (inUsers >> fileUserName){
        matchPos++;
        filePos = matchPos;
        if (fileUserName == userInput) {
            return matchPos;
        }
    }
    inUsers.close();
    return -1;
}

// check if password matches
bool User::CheckPassword(int userPos, std::string inputPass) {
    std::ifstream inPass;
    std::string passTest;
    int passwordPos = 0;
    
    inPass.open("passwords.txt");

    while (inPass >> passTest){
        passwordPos++;
        if (passTest == inputPass && userPos == passwordPos) {
            return true;
        }
    }
    return false;
}

// attempt to register user if not found in usernames.txt
bool User::RegisterUser(std::string userInput, std::string userPass) {
    if (AlreadyRegistered(userInput)) {
        std::ofstream outUsers;
        std::ofstream outPass;
        outUsers.open("usernames.txt",std::ofstream::app);
        outPass.open("passwords.txt",std::ofstream::app);

        outUsers << userInput << std::endl;
        outPass << userPass << std::endl;

        outUsers.close();
        outPass.close();
        return true;
    }
    return false;

}

// check if in usernames.txt
bool User::AlreadyRegistered(std::string userInput) {
    std::ifstream inUser;
    std::string currUser;
    inUser.open("usernames.txt");
    while (inUser >> currUser) {
        if (userInput == currUser) {
            return false;
        }
    }
    return true;
}

// send locations to client
void User::SendLocations() {
    char server_message[2000];// client_message[2000];
    std::string allLocations = "Locations subscribed to are:\n";
    //std::cout << "size: " << locations.size() << std::endl;

    for (long unsigned int i = 0; i < locations.size(); i++) {
        allLocations.append(locations.at(i));
        allLocations.append("\n");
    }

    //std::cout << allLocations << std::endl;
    strcpy(server_message,allLocations.c_str());
    write(sock, &server_message, strlen(server_message));
}

void User::SendMessage(std::string location) {
// TODO: for part 2
}

// add location to local location vector
void User::SuscribeLocation(std::string location) {
    for (long unsigned int i = 0; i < locations.size(); i++) {
        if (locations.at(i) == location) {
            return;
        }
    }
    locations.push_back(location);
}

// remove location from location vector
void User::UnuscribeLocation(std::string location) {
   for (long unsigned int i = 0; i < locations.size(); i++) {
        if (locations.at(i) == location) {
            locations.erase(locations.begin() + i);
        }
    }
}