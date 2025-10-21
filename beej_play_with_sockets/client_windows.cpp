/*
** client_windows.cpp
**
** Windows version on C++ - a stream socket client demo
*/

#include <iostream>
#include <string>
#include <system_error>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "SimpleSocket.h"

#define MAX_DATA_SIZE 100

int main(int argc, char* argv[])
{
    try {
        if (argc != 2) {
            throw std::invalid_argument("invalid argument, usage: showip hostname");
        }

        addrinfo addr_info{};
        auto client = su::SimpleSocket::createConnectedSocket(argv[1], "3490", &addr_info); // call factory to connect

        if (client) {
            std::cout << "client: connected to " << su::SimpleAddrinfo::getIP(addr_info) << std::endl;
        }

        char buf[MAX_DATA_SIZE];
        int numbytes = -1;
        if ((numbytes = client->recv(buf, MAX_DATA_SIZE-1)) == -1) {
            std::cerr << "recv" << std::endl;
	    } else {
            buf[numbytes] = '\0';
            std::cout << "client: received " << buf << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}