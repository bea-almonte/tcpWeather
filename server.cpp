// Bea Almonte
// httpServer.cpp
// This code creates an HTTP server capable of parsing requests
// and sending files

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <sstream> 
#include <arpa/inet.h> // inet_addr

#define PORT 60001
typedef struct {
    int sock; // socket of client
    struct sockaddr address; // address of client
    unsigned int addr_len;   // length of address
} connection_t;

std::string parseResponse(char * buffer); // parse request from client
std::string findLength(char * fileName); // find length of buffer
void * Process(void * ptr); // create socket
void chooseFile(std::string fileName); // check if file exists, send file
void sendFile(char * fileName,int &sock); // sends file through socket

int main(int argc, char** argv) {
    int sock = -1;
    int port = 60001;
    struct sockaddr_in address;
    connection_t * connection;
    pthread_t thread;
    // Setting port number, check for arguments
    if (argc > 1)
    {
        port = std::stoi(argv[1]);
        if (port < 60001 || port > 60099) {
            port = 60001;
            std::cout << "Invalid port input, setting to default port:" << port << std::endl << std::endl;
        } else {
            std::cout << "Setting port to: " << port << std::endl << std::endl;
        }
        
    } else {
        port = 60000;
        std::cout << "Setting to default port: " << port << std::endl << std::endl;
    }
    
    // create socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock <= 0) {
        std::cerr << "Error cannot create socket.\n";
        return -1;
    } 

    // bind socket to port
    std::cout << "Binding socket to port.\n\n";
    address.sin_family = AF_INET;
    //address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(60000);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(sock, (struct sockaddr*)&address,sizeof(struct sockaddr_in)) < 0) {
        std::cerr << "Error: cannot bind socket to port.\n";
        return -2;
    }

    // listen to port
    if (listen(sock,5) < 0) {
        std::cerr << "Error: Cannot listen on port.\n";
        return -3;
    }

    std::cout << "Server ready and listening.\n";

    while (true) {
        // accept connection
        connection = (connection_t *)malloc(sizeof(connection_t));
        connection->sock = accept(sock, &connection->address, &connection->addr_len);
        if (connection->sock <= 0) {
            free(connection);
        } else {
            std::cout << "\n===============================\n";
            std::cout << "Created thread.\n";
            pthread_create(&thread,0,Process, (void*)connection);
            pthread_detach(thread);
        }
    }
}

void * Process(void * ptr) {
    char * buffer = new char[4096];
    char clientStr[2000];
    connection_t * conn;

    if (!ptr) {
        pthread_exit(0);
    }
    
    conn = (connection_t *)ptr;
    
    // clear buffer
    memset(buffer,0,4096);
/*     // read request
    read(conn->sock, buffer, 4096);

    // read file and send
    chooseFile(parseResponse(buffer),conn->sock); */
    buffer = "Send me messages, client.";
/*     while (buffer != "0") {
        send(conn->sock, buffer, strlen(buffer), 0);
        memset(buffer,0,4096);
        if (recv(conn->sock, buffer, sizeof(buffer), 0) < 0){
            printf("Couldn't receive\n");
            continue;
        } else {
            printf("Msg from client: %s\n", buffer);
        }
    }
     */
    std::cout << "Sock: " << conn->sock << std::endl;
    for (int i = 0; i < 5; i++) {
        send(conn->sock, buffer, strlen(buffer), 0);
        memset(clientStr,0,2000);
        recv(conn->sock, clientStr, sizeof(clientStr), 0);
        printf("Msg from client: %s\n", clientStr);   
    }
    
    //delete[] buffer;
    
    close(conn->sock);
    free(conn);
    pthread_exit(0);
}

// send file from parsed input
void sendFile(char * fileName,int &sock) {
    char * fileBuffer = new char[4096];
    long long bytesRecv = 0;
    long long totSent = 0;
    int fd1 = open(fileName, O_RDONLY, 0); 

    // clear buffer
    memset(fileBuffer,0,4096);

    // attach header 
    sprintf(fileBuffer,"HTTP/1.1 200 OK\r\n\r\n");
    // send header
    write(sock,fileBuffer,strlen(fileBuffer));
    
    // check if file can be opened
    if (fd1 == -1) {
        std::cout << "Error: Cannot open file.\n";
        
    }
    
    std::cout << "\nAttempting to transmit " << findLength(fileName) << " bytes.\n";

    // keep sending as much memory in file until all bytes are sent
    do {
        memset(fileBuffer,0,4096);
        bytesRecv = read(fd1, fileBuffer, 4096);
        totSent += bytesRecv;
        write(sock,fileBuffer,bytesRecv);
    } while (bytesRecv > 0);
    
    std::cout << "Successfully transmitted " << totSent << " bytes.\n\n";
    // deallocate char
    delete[] fileBuffer;
}

// parse request from client
std::string parseResponse(char * buffer) {
    std::stringstream ss;
    std::string request;
    std::string fileName;

    // GET
    ss << buffer;
    ss >> request;
    // file name requested
    ss << buffer;
    ss >> fileName;
    // delete '/' if not first page
    std::cout << "\nREQUEST: " << request << " " << fileName << std::endl;
    if (fileName.length() > 1) {
        fileName.erase(0,1);
    }

    return fileName;
}

// find length of file to be sent
std::string findLength(char * fileName) {
    std::string stringLength;
    char * fileBuffer = new char[4096];
    int fd1 = open(fileName, O_RDONLY, 0); 
    int file_len = 0;
    int bytesRecv = 0;
    
    // clear buffer
    memset(fileBuffer,0,4096);

    // keep reading until all bytes are read
    do {
        memset(fileBuffer,0,4096);
        bytesRecv = read(fd1, fileBuffer, 4096);
        file_len += bytesRecv;
    } while (bytesRecv > 0);

    // convert length to string
    stringLength = std::to_string(file_len);

    delete[] fileBuffer;
    return stringLength;
}

// attempt to open or send error
void chooseFile(std::string fileName, int &sock){
    char * charName = new char[fileName.size()+1];
    char * error = "HTTP/1.0 404 Not Found\r\n\r\n<h1>404 Not Found</h1>";

    // change filename into cstring
    std::strcpy(charName,fileName.c_str());

    std::cout << "Attempting to open " << charName << std::endl;
    if (open(charName, O_RDONLY, 0) == -1) {
        send(sock,error,strlen(error),0);
        std::cout << "File " << charName << " doesn't exist. Sending error message.\n";
    } else if (fileName == "/") {
        // send html front
        sendFile("front.html",sock);
    } else {
        //send(sock , response , strlen(response) , 0 );
        std::cout << "Sending " << fileName << std::endl;
        sendFile(charName,sock);
    }

    // deallocate charNAme
    delete[] charName;
    
}

