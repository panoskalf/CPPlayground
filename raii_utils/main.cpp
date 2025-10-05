// Platform-agnostic main function
#include "try_sockets_common.h"
#include <iostream>
#include <string>

// Include platform-specific headers for the recv function
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <errno.h>
#endif

int main() {
    std::cout << "Starting cross-platform socket client..." << std::endl;

    // Option 1: Use the RAII class
    SocketClient client;

    // Option 2: Use free functions (for compatibility with your original code)
    int s = connect_to_host("example.com", "80");
    if (s < 0) {
        std::cerr << "Failed to connect" << std::endl;
        return 1;
    }

    std::string req = "GET / HTTP/1.1\r\nHost: example.com\r\nConnection: close\r\n\r\n"; // Fixed HTTP version
    if (send_all(s, req.c_str(), req.size()) < 0) {
        std::cerr << "send failed" << std::endl;
        client.close_socket(s);
        return 1;
    }

    char buf[1024];
    std::cout << "Response from server:" << std::endl;
    std::cout << "=====================" << std::endl;

    while (true) {
        int r;
#ifdef _WIN32
        r = recv(s, buf, sizeof(buf) - 1, 0);
        if (r == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if (error == WSAEINTR) continue;
            break;
        }
#else
        r = recv(s, buf, sizeof(buf) - 1, 0);
        if (r < 0) {
            if (errno == EINTR) continue;
            break;
        }
#endif
        if (r == 0) break; // peer closed
        buf[r] = '\0';
        std::cout << buf;
    }

    client.close_socket(s);
    std::cout << std::endl << "Connection closed." << std::endl;
    return 0;
}