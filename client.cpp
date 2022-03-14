// bea almonte
// Due: 3/13/21
// cliet.cpp
// runs the client object

#include "tcpClient.hpp"

int main(int argc, char ** argv)
{
    tcpClient Client;
    
    if (argc > 1)
    {
        Client.SetPort(std::stoi(argv[1]));
    } else {
        Client.SetPort(60022);
    }
    
    // Connect to server using input port
    if (!Client.ConnectToServer()) {
        return -1;
    }
    
    // login
    Client.Login();

    // create thread to listen to server
    // send commands to server
    if (!Client.endProgram) {
        Client.SendInput();
    }
    
    /* close socket */
    Client.CloseSocket();

    return 0;
}