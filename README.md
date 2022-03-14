# HTTP Server Project - Bea Almonte
## Description

This project implements an HTTP server to be run by a browser client.


**The submission includes the following files:**


|                      |                    |
| -------------------- | -------------------|
| location.cpp         | location.o         |
| user.cpp             | user.o             |
| tcpClient.cpp        | tcpClient.o        |
| tcpServer.cpp        | tcpServer.o        |
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