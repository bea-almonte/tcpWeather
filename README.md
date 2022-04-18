# TCP Server Project - Bea Almonte
## Description

This project implements an TCP server and client that will communicated with the server. The server will take the client requests. The client takes requests from the user.


**The submission includes the following files:**


|                      |                    |
| -------------------- | -------------------|
| location.cpp         | location.hpp       |
| user.cpp             | user.hpp           |
| tcpClient.cpp        | tcpClient.hpp      |
| tcpServer.cpp        | tcpServer.hpp      |
| server.cpp           | client.cpp         |
| usernames.txt        | passwords.txt      |
| Makefile             | README.md          |



## Compiling the Program
- At the command line, issue the command `make server` and `make client` to compile the server and client
- The program produces an executable entitled `server` and `client`
- The command `make clean` will delete the object files and executable

## Running the Server
- Use the command `./server [PORT NUMBER]`
- `[PORT NUMBER]` is an optional argument
- `[PORT NUMBER]` must be between  60001 - 60099
- Running it without an argument or invalid argument will set the port to `60001`

## Running the Client
- Make sure the server is already running.
- Use the command `./client [PORT NUMBER]`
- `[PORT NUMBER]` must be the same as the one used for the server
- `[PORT NUMBER]` is an optional argument
- `[PORT NUMBER]` must be between  60001 - 60099
- Running it without an argument or invalid argument will set the port to `60001`

## Inputs
- The user will not interact with the server.
- Any input must go to the `client` program and must not have any whitespaces

## Expected Outputs
- The program will output on the console.

### Login
- The user can login using usernames that are registered with the corresponding password.
- The user can register if their username is not in the text file.

### Subscribe
- The user will be able to subscribe and unsubscribe to locations.

### Change Password
- The program will changed the user password using a input with no whitespace.

## Send Messages to location
- An output of all subscribed locations are displayed. User has to input the location and then the message to send it. All users except the one sending it will receive the message.

## Send Private Message to User
- An output of all online users are displayed. User has to input the username of the recepient and then the message.