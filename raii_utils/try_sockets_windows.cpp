// Windows implementation
#include "try_sockets_common.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <cstring>

// Link libraries automatically
#pragma comment(lib, "ws2_32.lib")

SocketClient::SocketClient() : initialized(false) {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return;
    }
    initialized = true;
}

SocketClient::~SocketClient() {
    if (initialized) {
        WSACleanup();
    }
}

int SocketClient::connect_to_host(const std::string& host, const std::string& port) {
    if (!initialized) return -1;
    return ::connect_to_host(host, port);
}

int SocketClient::send_all(int fd, const char* data, size_t len) {
    return ::send_all(fd, data, len);
}

void SocketClient::close_socket(int fd) {
    if (fd != INVALID_SOCKET) {
        closesocket(fd);
    }
}

// Free function implementations (Windows versions)
int connect_to_host(const std::string& host, const std::string& port) {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP

    addrinfo* res = nullptr;
    int err = getaddrinfo(host.c_str(), port.c_str(), &hints, &res);
    if (err) {
        std::cerr << "getaddrinfo failed: " << WSAGetLastError() << std::endl;
        return -1;
    }

    SOCKET sock = INVALID_SOCKET;
    for (addrinfo* p = res; p != nullptr; p = p->ai_next) {
        sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock == INVALID_SOCKET) {
            continue;
        }
        if (connect(sock, p->ai_addr, p->ai_addrlen) == 0) {
            break; // success
        }
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
    freeaddrinfo(res);
    return (sock == INVALID_SOCKET) ? -1 : static_cast<int>(sock);
}

int send_all(int fd, const char* data, size_t len) {
    size_t total = 0;
    while (total < len) {
        int n = send(fd, data + total, static_cast<int>(len - total), 0);
        if (n == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if (error == WSAEINTR) continue;
            if (error == WSAEWOULDBLOCK) {
                continue;
            }
            return -1;
        }
        total += static_cast<size_t>(n);
    }
    return static_cast<int>(total);
}