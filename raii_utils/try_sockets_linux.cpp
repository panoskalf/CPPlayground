// Linux/POSIX implementation
#include "try_sockets_common.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <errno>

SocketClient::SocketClient() : initialized(true) {
    // No special initialization needed on Linux
}

SocketClient::~SocketClient() {
    // No special cleanup needed on Linux
}

int SocketClient::connect_to_host(const std::string& host, const std::string& port) {
    return ::connect_to_host(host, port);
}

int SocketClient::send_all(int fd, const char* data, size_t len) {
    return ::send_all(fd, data, len);
}

void SocketClient::close_socket(int fd) {
    if (fd >= 0) {
        close(fd);
    }
}

// Free function implementations (your original code)
int connect_to_host(const std::string& host, const std::string& port) {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP

    addrinfo* res = nullptr;
    int err = getaddrinfo(host.c_str(), port.c_str(), &hints, &res);
    if (err) {
        std::cerr << "getaddrinfo: " << gai_strerror(err) << std::endl;
        return -1;
    }

    int sock = -1;
    for (addrinfo* p = res; p != nullptr; p = p->ai_next) {
        sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock < 0) {
            continue;
        }
        if (connect(sock, p->ai_addr, p->ai_addrlen) == 0) {
            break; // success
        }
        close(sock);
        sock = -1;
    }
    freeaddrinfo(res);
    return sock; // -1 on failure
}

int send_all(int fd, const char* data, size_t len) {
    size_t total = 0;
    while (total < len) {
        ssize_t n = send(fd, data + total, len - total, MSG_NOSIGNAL);
        if (n < 0) {
            if (errno == EINTR) continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                continue;
            }
            return -1;
        }
        total += static_cast<size_t>(n);
    }
    return static_cast<int>(total);
}