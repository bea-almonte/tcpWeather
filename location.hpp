// bea almonte
// Due: 4/17/22
// location.hpp
// contains the name of location and vector to hold the
// usernames and sockets of subscribers
#include <iostream>
#include <string>
#include <vector>

#ifndef LOCATION_HPP
#define LOCATION_HPP

// if locName doesn't match
// create new
// subscribe new user inside constructor
class Location {
    public:
        int totalSubscribed;
        std::string locName; // location name
        std::vector<std::string> nameSubbed;
        std::vector<int> socketSubbed;
        void BroadcastMessage(std::string sender, std::string message);
        bool subscribeUser(std::string username, int socket); // push_back username if not found
        void unsubscribeUser(std::string username); // erase matching username
        bool alreadySubscribed(std::string username);
        void DisplayUsers();
};

#endif