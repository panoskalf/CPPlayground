/*
** showip_windows.c
**
** Windows version on C++ - show IP addresses for a host given on the command line
*/

#include <iostream>
#include <string>
#include <system_error>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "SimpleSocket.h"

int main(int argc, char* argv[])
{
    try {
        if (argc != 2) {
            throw std::invalid_argument("invalid argument, usage: showip hostname");
        }

        su::SimpleAddrinfo addr_info(argv[1], std::string(), AF_UNSPEC, SOCK_STREAM, AI_ALL | AI_V4MAPPED);

        std::cout << "IP addresses for " << argv[1] << ":" << std::endl;

        for(const auto& addr : addr_info) {
            std::cout << "Processing family: " << addr.ai_family << std::endl;
            std::cout << "  " << su::SimpleAddrinfo::getIPVersion(addr) << ": "
                      << su::SimpleAddrinfo::getIP(addr) << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}