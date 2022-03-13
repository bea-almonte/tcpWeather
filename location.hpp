#include <iostream>
#include <string>
#include <vector>

#ifndef LOCATION_HPP
#define LOCATION_HPP

class Location {
    public:
        int totalSubscribed;
        std::string locName; // location name
        std::vector<std::string> usernames;
        std::vector<int> clientSockets;
        void BroadcastMessage(std::string sender, std::string message);
};

#endif