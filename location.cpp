// bea almonte
// Due: 4/17/22
// contains functions to hold the username subscribed to location

#include "location.hpp"

void Location::BroadcastMessage(std::string sender, std::string message) {
    std::cout << "Feature not available yet.\n";
}

// push_back username if not found
bool Location::subscribeUser(std::string username, int socket) {
    if (alreadySubscribed(username)) {
        std::cout << username << " already subscribed to " << locName << "\n";
        return false;
    }
    nameSubbed.push_back(username);
    socketSubbed.push_back(socket);
    totalSubscribed++;
    return true;
} 

// erase username from vector
void Location::unsubscribeUser(std::string username) {
    for (long unsigned int i = 0; i < nameSubbed.size();i++) {
        if (username == nameSubbed[i]) {
            nameSubbed.erase(nameSubbed.begin() + i);
            socketSubbed.erase(socketSubbed.begin() + i);
            totalSubscribed--;
            return;
        }
    }
    
} 

bool Location::alreadySubscribed(std::string username) {
    for (long unsigned int i = 0; i < nameSubbed.size();i++) {
        if (username == nameSubbed[i]) {
            return true;
        }
    }
    return false;
}

void Location::DisplayUsers() {
    std::cout << "Users subscribed to \'" << locName << "\' are: " << std::endl;

    for (long unsigned int i = 0; i < nameSubbed.size();i++) {
        std::cout << nameSubbed[i] << " ";
    }
    std::cout << "\n";
}