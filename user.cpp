#include "user.hpp"

bool User::Login() {
    int bytesRec, code, pos;
    exitUser = false;
    std::string inputName, inputPass;

    // clear buffer
    memset(client_message,0,2000);
    memset(server_message,0,2000);

    bytesRec = recv(sock, client_message, sizeof(client_message), 0);
    std::string wholeThing(client_message);
   
    std::cout << "Attempting to login.\n";
    std::cout << "Received: " << bytesRec << std::endl;
    // PARSE INPUT
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

    // Testing
    std::cout << "Code: " << code << std::endl;
    std::cout << inputName << " | " << inputPass << std::endl;

    // Set username and pass
    username = inputName;
    password = inputPass;


    if (code == 1) {
        // check username

        // check password
        sprintf(server_message,"100");
        write(sock, &server_message, strlen(server_message));
        return true;
    } else if (code == 2) {
        // register
        RegisterUser(username, password);
        sprintf(server_message,"200");
        write(sock, &server_message, strlen(server_message));
        return false;
    } else {
        // Bad request
        sprintf(server_message,"203");
        write(sock, &server_message, strlen(server_message));
    }

    return false;

}

void User::SetUsername(std::string userInput) {

}

// output position of
int User::CheckUsername(std::string userInput) {
    std::ifstream inUsers;
    std::string fileUserName;
    int matchPos = 0;
    inUsers.open("usernames.txt");
    while (inUsers >> fileUserName){

    }
    return -1;
}

void User::RegisterUser(std::string userInput, std::string userPass) {
    std::ofstream outUsers;
    std::ofstream outPass;
    outUsers.open("usernames.txt",std::ofstream::app);
    outPass.open("passwords.txt",std::ofstream::app);

    outUsers << userInput << std::endl;
    outPass << userPass << std::endl;

    outUsers.close();
    outPass.close();
}

void User::PrintLocations() {

}
void User::SendMessage(std::string location) {

}
void User::SuscribeLocation(std::string location) {

}
void User::UnuscribeLocation(std::string location) {

}