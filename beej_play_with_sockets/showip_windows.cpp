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
    explicit AddrinfoRAII(const char* hostname) {
        struct addrinfo hints{}; // Initialize hints to zero
        hints.ai_family = AF_UNSPEC; // Either IPv4 or IPv6
        hints.ai_socktype = SOCK_STREAM; // TCP
        hints.ai_flags = AI_ALL | AI_V4MAPPED; // Request both IPv4 and IPv6 results

        int status = getaddrinfo(hostname, nullptr, &hints, &info_);
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

int main(int argc, char* argv[])
{
    try {
        if (argc != 2) {
            throw std::invalid_argument("invalid argument, usage: showip hostname");
        }

        WinsockRAII winsock;
        AddrinfoRAII addr_info(argv[1]);

        std::cout << "IP addresses for " << argv[1] << ":" << std::endl;

        for(const auto& addr : addr_info) {
            std::cout << "Processing family: " << addr.ai_family << std::endl;
            void *addr_ptr;
            std::string ipver;

            // get the pointer to the address itself
            // different fields in IPv4 and IPv6
            if (addr.ai_family == AF_INET) { // IPv4
                auto* ipv4 = reinterpret_cast<struct sockaddr_in *>(addr.ai_addr);
                addr_ptr = &(ipv4->sin_addr);
                ipver = "IPv4";
            } else if (addr.ai_family == AF_INET6) { // IPv6 (should be 23 on Windows)
                auto* ipv6 = reinterpret_cast<struct sockaddr_in6 *>(addr.ai_addr);
                addr_ptr = &(ipv6->sin6_addr);
                ipver = "IPv6";
            } else {
                // Expected failure just log
                std::cerr << "  Unknown address family: " << addr.ai_family << std::endl;
                continue;
            }

            // convert the IP to a string and print it:
            char ipstr[INET6_ADDRSTRLEN];
            if (inet_ntop(addr.ai_family, addr_ptr, ipstr, sizeof(ipstr))) {
                std::cout << "  " << ipver << ": " << ipstr << std::endl; // DEBUG ouput
            } else {
                throw std::runtime_error("Failed to convert " + ipver + " address to string");
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}