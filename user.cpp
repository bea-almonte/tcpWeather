#include "user.hpp"

bool User::Login() {
    int bytesRec, code, pos;
    char server_message[2000], client_message[2000];
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


    // 100 succesful login
    // 102 right username, wrong pass
    // 103 user does not exist
    // TODO: 104 user already logged in
    if (code == 1) {
        // check username
        if (CheckUsername(username) >= 0) {
            if (CheckPassword(CheckUsername(username),password)) {
                //std::cout << "This should be...\n";
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

void User::WaitRecv(int socket) {
    char server_message[2000], client_message[2000];
    int received = 0;
    int pos = -1;
    char code = 'x';
    while (!exitUser) {
        memset(client_message,0,2000);
        received = recv(socket, client_message, sizeof(client_message), 0);
        if (received <= 0) {
            std::cout << "Error: exiting user\n";
            exitUser = true;
            break;
        }
        std::string request(client_message);
        std::cout << "uhm: " << received << std::endl;
        std::cout << "string: " << request << std::endl;
        code = request[0];
        std::cout << "g\n";
        std::cout << username << "'s request: " << code << std::endl;
        
        if (received == 0) {
            break;
        }
        switch (code) {
            case '0':
                std::cout << "Exiting...\n";
                exitUser = true;
                break;
            case '1':
                std::cout << "Subsrcribe to a location\n";
                break;
            case '2':
                std::cout << "Unsub from location\n";
                break;
            case '3':
                std::cout << "Display locations subbed\n";
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
                // send error msg
                break;

        }
        //std::cout << "Bytes: " << received << std::endl;

        std::string test(client_message);
        if (test == "exit") {
            exitUser = true;
        }
    }
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
        matchPos++;
        filePos = matchPos;
        if (fileUserName == userInput) {
            return matchPos;
        }
    }
    inUsers.close();
    return -1;
}
// check 
bool User::CheckPassword(int userPos, std::string inputPass) {
    std::ifstream inPass;
    std::string passTest;
    int passwordPos = 0;
    
    inPass.open("passwords.txt");

    while (inPass >> passTest){
        passwordPos++;
        if (passTest == inputPass && userPos == passwordPos) {
            return true;;
        }
    }
    return false;
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