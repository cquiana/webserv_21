

/*
** This is a realisation of this tutorial, showing how to build
** a simple HTTP server in C.
** https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa
**
** THIS IS SERVER SIDE.
** The steps are :
** 1) opening a socket (an access to the network)
** 2) giving this socket an address (a port, with which we bind it)
** 3) listening for a connection
** 4) accepting a connection and creating a new socket for it
** 5) read client message, send a message back
** 6) close the communication socket
*/

#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#define PORT 8888 // default port



int main() {

    /*
    ** 1) CREATE A SOCKET
    */

    int sock_fd;
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        strerror(errno);
        return 0;
    }

    // to allow for fast restart, otherwise binding fails because port is still in use from previous attempt
    int ret = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&ret, sizeof(ret)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");
    /*
    ** 2) NAME A SOCKET
    */

    sockaddr_in address;
    memset((char *)&address, 0, sizeof(address));
    int addrlen = sizeof(address);
    address.sin_family = AF_INET; // the IPv4 domain we used to open the socket
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT); // htons() = short int to network
    address.sin_len = addrlen; // just the size of the struct
    memset(address.sin_zero, '\0', sizeof(address.sin_zero));

    if (bind(sock_fd, (struct sockaddr *)&address, addrlen) < 0) {
        strerror(errno);
        exit(EXIT_FAILURE);
    }

    /*
    ** 3) LISTEN
    */

    if (listen(sock_fd, 3) < 0) {
       strerror(errno);
       exit(EXIT_FAILURE);
    }

    /*
    ** 4) ACCEPT A CONNECTIO
     * N AND CREATE A NEW SOCKET FOR IT
    */

    int new_sock;

    while (1) {
        std::cout << std::endl << "[--- WAITING FOR NEW CONNECTION ---]" << std::endl;
        if ((new_sock = accept(sock_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            strerror(errno);
            exit(EXIT_FAILURE);
        }

        /*
        ** 5) SEND AND RECEIVE MESSAGES
        ** The same read and write system calls that work on files also work on sockets.
        */

        // I think we'll have to use GNL?
        char buffer[1024] = {0};
        int was_red = read(new_sock , buffer, 1024);
        std::cout << "[CLIENT MSG] " << buffer << std::endl;
        if(was_red < 0)
            std::cout << "[No bytes are there to read]" << std::endl;
        // we are gonna send a message with a proper HTTP header format (or the browser wouldn't accept it)
        char hello[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 10\n\nHello world!";//IMPORTANT! WE WILL GET TO IT
        write(new_sock , hello , strlen(hello));
        std::cout << "[--- HELLO MSG SENT ---]" << std::endl;

        /*
        ** 6) CLOSE THE SOCKET
        ** The same close() that we use for files
        */

        close(new_sock);
    }

    return 0;
}
