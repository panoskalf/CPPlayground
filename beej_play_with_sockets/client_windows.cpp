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

// RAII wrapper for Winsock initialization
class WinsockRAII {
public:
    WinsockRAII() {
        WSADATA wsaData;
        const int result = WSAStartup(MAKEWORD(2,2), &wsaData);
        // Initialize Winsock - windows specific
        if (result != 0) {
            throw std::system_error(result, std::system_category(), "WSAStartup failed");
        }
    }
    ~WinsockRAII() {
        WSACleanup();
    }
    // Non-copyable, non-movable (implicitly) for simplicity
    WinsockRAII(const WinsockRAII&) = delete;
    WinsockRAII& operator=(const WinsockRAII&) = delete;
};

// RAII wrapper for addrinfo to ensure correct cleanup
class AddrinfoRAII {
    addrinfo* info_ = nullptr;
public:
    explicit AddrinfoRAII(const char* hostname, const char* port) {
        struct addrinfo hints{}; // Initialize hints to zero
        hints.ai_family = AF_UNSPEC; // Either IPv4 or IPv6
        hints.ai_socktype = SOCK_STREAM; // TCP
        hints.ai_flags = AI_ALL | AI_V4MAPPED; // Request both IPv4 and IPv6 results

        int status = getaddrinfo(hostname, port, &hints, &info_);
        if (status != 0) {
            throw std::runtime_error("getaddrinfo failed with code: " + std::to_string(status));
        }

        if (info_ == nullptr){
            throw std::runtime_error("No address information returned");
        }
    }
    ~AddrinfoRAII() {
        if (info_)
            freeaddrinfo(info_); // free the linked list
    }
    // Non-copyable, non-movable (implicitly) for simplicity
    AddrinfoRAII(const AddrinfoRAII&) = delete;
    AddrinfoRAII& operator=(const AddrinfoRAII&) = delete;

    // Simple wrapper that works with std algorithms (for range for)
    struct iterator {
        addrinfo* ptr;
        addrinfo& operator*() const { return *ptr; }
        addrinfo* operator->() const { return ptr; }
        iterator& operator++() { ptr = ptr ? ptr->ai_next : nullptr; return *this; }
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }
        bool operator==(const iterator& other) const { return ptr == other.ptr; }
    };

    // Range inference
    iterator begin() const noexcept { return {info_}; }
    iterator end() const noexcept { return {nullptr}; }
    // Direct access
    addrinfo* get() const noexcept { return info_; }
};

#define MAXDATASIZE 100 // max number of bytes we can get at once

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char* argv[])
{
    try {
        if (argc != 2) {
            throw std::invalid_argument("invalid argument, usage: showip hostname");
        }

        WinsockRAII winsock;
        AddrinfoRAII addr_info(argv[1], "3490");

        std::cout << "IP addresses for " << argv[1] << ":" << std::endl;
        SOCKET sock{};

        bool connected = false;
        for(const auto& addr : addr_info) {
            sock = socket(addr.ai_family, addr.ai_socktype, addr.ai_protocol);
            if (sock == INVALID_SOCKET) {
                std::cerr << "Socket failed, family: " << addr.ai_family << std::endl;
                continue;
            }

            char s[INET6_ADDRSTRLEN];
            inet_ntop(addr.ai_family,
            get_in_addr((struct sockaddr *)addr.ai_addr), s, sizeof(s));
            std::cout << "client: attempting connection to " << s << std::endl;

            if (connect(sock, addr.ai_addr, addr.ai_addrlen) == SOCKET_ERROR) {
                std::cerr << "client: connect" << std::endl;
                closesocket(sock);
                continue;
		    }

	        std::cout << "client: connected to " << s << std::endl;
            connected = true;
            break;
        }

        if (!connected) {
            std::cerr << "client: failed to connect" << std::endl;
        }

        char buf[MAXDATASIZE];
        int numbytes = -1;
        if ((numbytes = recv(sock, buf, MAXDATASIZE-1, 0)) == -1) {
            std::cerr << "recv" << std::endl;
	    } else {
            buf[numbytes] = '\0';
            std::cout << "client: received " << buf << std::endl;
        }

        closesocket(sock);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}