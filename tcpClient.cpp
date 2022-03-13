#include "tcpClient.hpp"

tcpClient::tcpClient() {
    endProgram = false;
    port = 60000;
}

void tcpClient::SetPort(int userPort) {
    port = userPort;
}

void tcpClient::ConnectToServer() {
    /* create socket */
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock <= 0)
    {
        std::cout << "Error: cannot create socket " << sock << std::endl;
    }

    /* connect to server */
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");


    if (connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cout << "Unable to connect\n";
    }
}

void tcpClient::Login() {
    bool loggedIn = false;
    std::string input;
    int sent = 0;
    std::string usernameInput;
    std::string password;
    
    std::cout << "\nWelcome!\n";
    
    while (!loggedIn) {
        LoginOutput();
        std::cin >> input;
        memset(client_message,0,2000);
        memset(server_message,0,2000);
        if (input == "exit") {
            sprintf(client_message,"3");
            sent = write(sock, &client_message, strlen(client_message)+1);
            endProgram = true;
            break;
        } else if (input == "1") {
            std::cout << "Username: ";
            std::cin >> usernameInput;
            username = usernameInput;
            std::cout << "Password: ";
            std::cin >> password;
            
            // send code user password

            // code username password
            sprintf(client_message,"1 %s %s",usernameInput.c_str(),password.c_str());
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
            std::cin >> usernameInput;
            std::cout << "Password: ";
            std::cin >> password;
            username = usernameInput;

            // send code username password
            sprintf(client_message,"2 %s %s",usernameInput.c_str(),password.c_str());
            std::cout << client_message;
            write(sock, &client_message, strlen(client_message));
            // response from server
            recv(sock, server_message, sizeof(server_message), 0);
            std::string test(server_message);

            if (test == "200") {
                std::cout << "Succesfully Registered.\n";
            } else {
                std::cout << "Bad server response.\n";
            }
        }
        
    }
}

void tcpClient::LoginOutput() {
    std::cout << "\tPress 1 to Login\n";
    std::cout << "\tPress 2 to Register\n";
    std::cout << "\tType 'exit' to Quit\n";

}
void tcpClient::SendInput() {
    bool exitUser = false;
    std::string input;
    std::thread thread_(&tcpClient::Receive, this);

    thread_.detach();
    while (!exitUser) {
        OutputMenu();
        memset(client_message,0,2000);
        //std::cout << "Client> ";
        std::cin >> input;
        if (input == "exit") {
            strcpy(client_message,input.c_str());
            write(sock, &client_message, strlen(client_message));
            endProgram = true;
            break;
        } else {
            strcpy(client_message,input.c_str());
            write(sock, &client_message, strlen(client_message));
        }
    }
}
void tcpClient::CloseSocket() {
    close(sock);
}
void tcpClient::OutputMenu() {
    std::cout << "Select an option:\n";
    std::cout << " 1 Subsrcribe to a location\n";
    std::cout << " 2 Unsubsrcribe from a location\n";
    std::cout << " 3 All locations subcribed to\n";
    std::cout << " 4 Send a message to a location\n";
    std::cout << " 5 See all online users\n";
    std::cout << " 6 Send a message to a user\n";
    std::cout << " 7 Last 10 messages received\n";
    std::cout << " 8 Change password\n";
    std::cout << " 0 Logout\n";
}

void tcpClient::Receive() {
    memset(client_message,0,2000);
    std::cout << "Thread created.\n";
    sprintf(client_message,"From Created Thread.\n");
    write(sock, &client_message, strlen(client_message));
}