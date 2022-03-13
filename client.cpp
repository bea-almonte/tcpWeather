#include "tcpClient.hpp"

int main(int argc, char ** argv)
{
    tcpClient Client;
    
    if (argc > 1)
    {
        Client.SetPort(std::stoi(argv[1]));
    } else {
        Client.SetPort(60000);
    }
    
    // Connect to server using input port
    Client.ConnectToServer();
    
    // login
    Client.Login();

    // create thread to listen to server
    // send commands to server
    Client.SendInput();

    
    /* close socket */
    Client.CloseSocket();

    return 0;
}