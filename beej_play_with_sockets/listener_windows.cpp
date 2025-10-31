/*
** listener_windows.cpp
**
** Windows version on C++ - a datagram sockets "server" demo
*/

#include <iostream>
#include <string>
#include <system_error>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "SimpleSocket.h"

#define MYPORT "4950"    // the port users will be connecting to
#define MAXBUFLEN 100

int main(void)
{
    try {
        auto listener = su::SimpleSocket::createUdpListener(MYPORT, AF_UNSPEC);
        std::cout << "listener: waiting to recvfrom...\n" << std::endl;

        while(1) // continue to receive
        {
            int numbytes;
            char buf[MAXBUFLEN];
            struct sockaddr_storage their_addr;
            socklen_t addr_len = sizeof(their_addr);

            if ((numbytes = listener->recvfrom(buf, MAXBUFLEN-1 , 0,
                (struct sockaddr *)&their_addr, &addr_len)) == SOCKET_ERROR) {
                throw std::runtime_error("listener: recvfrom");
            }

            char s[INET6_ADDRSTRLEN];
            std::cout << "listener: got packet from" << su::SimpleAddrinfo::getIP(their_addr) << std::endl;

            std::cout << "listener: packet is " << numbytes << " bytes long\n";
            buf[numbytes] = '\0';
            std::cout << "listener: packet contains \"" << buf << "\"\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
