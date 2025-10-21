/*
** talker_windows.cpp
**
** Windows version on C++ - a datagram "client" demo
*/

#include <iostream>
#include <string>
#include <system_error>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "SimpleSocket.h"

#define MYPORT "4950"	// the port users will be connecting to
#define MAXBUFLEN 100

int main(int argc, char* argv[])
{
    if(argc != 3) {
        std::cerr << "usage: talker hostname message" << std::endl;
        exit(1);
	}

	try {
		su::SimpleAddrinfo addr(argv[1] , MYPORT, AF_INET6, SOCK_DGRAM, 0);
		su::SimpleSocket talker(*addr.get());

		std::string message{argv[2]};
		int bytes_sent = talker.sendto(message,
                    addr.get()->ai_addr, addr.get()->ai_addrlen);

        if (bytes_sent == SOCKET_ERROR) {
            throw std::runtime_error(talker.get_error());
	    }

        std::cout << "Sent " << bytes_sent << " bytes to " << argv[1] << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
