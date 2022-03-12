#include <iostream>
#include <string>

#ifndef LOCATION_HPP
#define LOCATION_HPP

class Location {
    public:
        int totalSubscribed;
        std::string usersSubscribed[5];
        int socketsConnected[5];
        void BroadcastMessage(std::string sender, std::string message);
};

#endif