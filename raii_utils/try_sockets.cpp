// robust_client.cpp (POSIX)
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <errno>

int connect_to_host(const std::string& host, const std::string& port) {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP

    addrinfo* res = nullptr;
    int err = getaddrinfo(host.c_str(), port.c_str(), &hints, &res); // ?
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

ssize_t send_all(int fd, const char* data, size_t len) {
    size_t total = 0;
    while (total < len) {
        ssize_t n = send(fd, data + total, len - total, MSG_NOSIGNAL);
        if (n < 0) {
            if (errno == EINTR) continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // if non-blocking, you might wait with poll/select here.
                continue;
            }
            return -1;
        }
        total += static_cast<size_t>(n);
    }
    return static_cast<ssize_t>(total);
}

int main() {
    int s = connect_to_host("example.com", "80");
    if (s < 0) {
        std::cerr << "Failed to connect" << std::endl;
        return 1;
    }

    std::string req = "GET / HTTP/ 1.1\r\nHost: example.com\r\nConnection: close\r\n\r\n";
    if (send_all(s, req.c_str(), req.size()) < 0) {
        std::cerr << "send failed" << std::endl;
        close(s);
        return 1;
    }

    char buf[1024];
    while (true) {
        ssize_t r = recv(s, buf, sizeof(buf) - 1, 0);
        if (r < 0) {
            if (errno == EINTR) continue;
            break;
        }
        if (r == 0) break; // peer closed
        buf[r] = '\0';
        std::cout << buf;
    }

    close(s);
    return 0;
}
