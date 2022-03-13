#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring> // memcpy
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>
#include "tcpClient.hpp"

void * Receive(void * ptr); // receive messages
bool endThis = false;
/* void Login(int sock);
void LoginOutput();
void OutputMenu();
void SendInput(int sock);
bool endProgram = false; */

int main(int argc, char ** argv)
{
    tcpClient Client;
    //pthread_t thread; // thread to keep listening
   /*  int port = 60000;
    int sock = -1;
    struct sockaddr_in address; */
    //char server_message[2000];
    //char client_message[2000];
    
    if (argc > 1)
    {
        Client.SetPort(std::stoi(argv[1]));
    } else {
        Client.SetPort(60000);
    }
    
    Client.ConnectToServer();
    
    Client.Login();

    /* pthread_create(&thread,0,Receive, &sock);
    pthread_detach(thread); */

    Client.SendInput();

    
    /* close socket */
    Client.CloseSocket();

    return 0;
}

/*void OutputMenu() {
    std::cout << "Select an option:\n";
    std::cout << " 1 Subsrcribe to a location\n";
    std::cout << " 2 Subsrcribe to a location\n";
    std::cout << " 3 Subsrcribe to a location\n";
    std::cout << " 4 Subsrcribe to a location\n";
    std::cout << " 5 Subsrcribe to a location\n";
    std::cout << " 6 Subsrcribe to a location\n";
    std::cout << " 7 Subsrcribe to a location\n";
    std::cout << " 8 Subsrcribe to a location\n";
    std::cout << " 0 Quit\n";

}

void LoginOutput() {
    std::cout << "\tPress 1 to Login\n";
    std::cout << "\tPress 2 to Register\n";
    std::cout << "\tType 'exit' to Quit\n";

}

void SendInput(int sock) {
    bool exitUser = false;
    char client_message[2000];
    std::string input;
    while (!exitUser) {
        memset(client_message,0,2000);
        std::cout << "Client> ";
        std::cin >> input;
        if (input == "exit") {
            sprintf(client_message,input.c_str());
            write(sock, &client_message, strlen(client_message));
            break;
        } else {
            sprintf(client_message,input.c_str());
            write(sock, &client_message, strlen(client_message));
        }
    }
}

void Login(int sock) {
    bool loggedIn = false;
    char server_message[2000];
    char client_message[2000];
    std::string input;
    int sent = 0;
    std::string username;
    std::string password;

    std::cout << "\nWelcome!\n";
    
    while (!loggedIn) {
        LoginOutput();
        std::cin >> input;
        if (input == "exit") {
            sprintf(client_message,"3");
            sent = write(sock, &client_message, strlen(client_message)+1);
            endProgram = true;
            break;
        } else if (input == "1") {
            std::cout << "Username: ";
            std::cin >> username;
            std::cout << "Password: ";
            std::cin >> password;
            // send code user password
            memset(client_message,0,2000);
            memset(server_message,0,2000);
            // code username password
            sprintf(client_message,"1 %s %s",username.c_str(),password.c_str());
            std::cout << "Sending: " << client_message << std::endl;
            sent = write(sock, &client_message, strlen(client_message));


            // server sends status reponse message
            std::cout << sent << std::endl;
            recv(sock, server_message, sizeof(server_message), 0);
            std::string test(server_message);

            // 100 successful login
            // 101 wrong username
            // 102 wrong password
            
            if (test == "100") {
                std::cout << "Succesfully Logged In.\n";
                loggedIn = true;
            } else if (test == "101") {
                std::cout << "RESPONSE: " << server_message << std::endl;
                std::cout << "Username not found.\n";
            } else if (test == "102") {
                std::cout << "Incorrect Password.\n";
            } else {
                std::cout << "RESPONSE: " << server_message << std::endl;
                std::cout << "Bad server response.\n";
            }

        } else if (input == "2") {
            std::cout << "Username: ";
            std::cin >> username;
            std::cout << "Password: ";
            std::cin >> password;
            // send code username password
            sprintf(client_message,"2 %s %s",username.c_str(),password.c_str());
            std::cout << client_message;
            write(sock, &client_message, strlen(client_message));
            recv(sock, server_message, sizeof(server_message), 0);
            std::string test(server_message);

            if (test == "200") {
                std::cout << "Succesfully Registered.\n";
            } else {
                std::cout << "Bad server response.\n";
            }
        }
        
    }
}*/

void * Receive(void * ptr) {
    char server_message[2000];
    //char client_message[2000];
    int sock = 0;
    std::string msg;
    sock = *(int*)ptr;

    while (!endThis) {
        recv(sock, server_message, sizeof(server_message), 0);
        printf("Server's response: %s\n",server_message);
        /* //memset(client_message,0,2000);
        ///std::cin >> msg;
        sprintf(client_message,msg.c_str());
        write(sock, &client_message, sizeof(client_message)); */
    }

    /* close socket */
    close(sock);

    pthread_exit(0);
}