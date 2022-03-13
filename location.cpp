#include "location.hpp"

void Location::BroadcastMessage(std::string sender, std::string message) {
    std::cout << "Feature not available yet.\n";
}

bool Location::subscribeUser(std::string username, int socket) {
    if (alreadySubscribed(username)) {
        std::cout << "Already subscribed.\n";
        return false;
    }
    nameSubbed.push_back(username);
    socketSubbed.push_back(socket);
    return true;
} // push_back username if not found

void Location::unsubscribeUser(std::string username) {
    for (long unsigned int i = 0; i < nameSubbed.size();i++) {
        if (username == nameSubbed[i]) {
            nameSubbed.erase(nameSubbed.begin() + i);
            socketSubbed.erase(socketSubbed.begin() + i);
            return;
        }
    }
    std::cout << "User doesn't exist.\n";
} // erase matching username

bool Location::alreadySubscribed(std::string username) {
    for (long unsigned int i = 0; i < nameSubbed.size();i++) {
        if (username == nameSubbed[i]) {
            return true;
        }
    }
    return false;
}

void Location::DisplayUsers() {
    std::cout << "Users subscribed to " << locName << std::endl;
    
    for (long unsigned int i = 0; i < nameSubbed.size();i++) {
        std::cout << nameSubbed[i] << " ";
    }
    std::cout << "\n";
}