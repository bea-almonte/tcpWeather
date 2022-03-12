#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring> // memcpy
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>

void * Receive(void * ptr); // receive messages
void Login(int sock);
void LoginOutput();
void OutputMenu();
bool endProgram = false;

int main(int argc, char ** argv)
{
    pthread_t thread; // thread to keep listening
    int port = 60000;
    int sock = -1;
    struct sockaddr_in address;
    char server_message[2000], client_message[2000];
    
    if (argc > 1)
    {
        port = std::stoi(argv[1]);
    }
    /* create socket */
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock <= 0)
    {
        fprintf(stderr, "%s: error: cannot create socket\n", argv[0]);
        return -3;
    }

    /* connect to server */
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");


    if (connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("Unable to connect\n");
        return -5;
    }

    // create user / login
    Login(sock);
    // password

 
    // create new thread to listen
    pthread_create(&thread,0,Receive, &sock);
    pthread_detach(thread);


    while (!endProgram) {
        // output options
        // select option
    }
    

    /* close socket */
    close(sock);

    return 0;
}

void LoginOutput() {
    std::cout << "\tPress 1 to Login\n";
    std::cout << "\tPress 2 to Register\n";
    std::cout << "\tType 'exit' to Quit\n";

}

void Login(int sock) {
    bool loggedIn = false;
    char outputBuffer[2000];
    std::string input;
    std::string username;
    std::string password;
    std::cout << "\nWelcome!\\n";
    
    while (!loggedIn) {
        LoginOutput();
        std::cin >> input;
        if (input == "exit") {
            endProgram = true;
            break;
        } else if (input == "1") {
            std::cout << "Username: ";
            std::cin >> username;
            std::cout << "Password: ";
            std::cin >> password;
            sprintf(outputBuffer,"1 %s %s",username.c_str(),password.c_str());
            std::cout << outputBuffer;
            write(sock, &outputBuffer, strlen(outputBuffer));
        }
        
    }
}

void * Receive(void * ptr) {
    char server_message[2000], client_message[2000];
    int sock = 0;
    std::string msg;
    sock = *(int*)ptr;

    while (!endProgram) {
        recv(sock, server_message, sizeof(server_message), 0);
        printf("Server's response: %s\n",server_message);
        /* //memset(client_message,0,2000);
        ///std::cin >> msg;
        sprintf(client_message,msg.c_str());
        write(sock, &client_message, sizeof(client_message)); */
    }

    /* close socket */
    close(sock);

    //delete[] buffer;
    
    close(sock);
    pthread_exit(0);
}