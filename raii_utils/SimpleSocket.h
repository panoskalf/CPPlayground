/**
 * @file SimpleSocket.h
 * @author Panos Kalf
 * @brief Simple platform agnostic socket wrapper API example in C++
 * @version 0.1
 * @date 2025-10-10
 *
 */

#pragma once
#include <string>
#include <memory>

// Forward declaration
struct addrinfo; // comes from Windows ws2tcpip.h - Linux netdb.h
struct sockaddr; // comes from Windows winsock2.h - Linux sys/socket.h
struct sockaddr_storage; // comes from Windows winsock2.h - Linux sys/socket.h

// Socket Utilities namespace abreviated
namespace su {
    class SimpleSocket {
    public:

        explicit SimpleSocket(int family, int socktype, int protocol);
        explicit SimpleSocket(const addrinfo& ai);
        ~SimpleSocket() noexcept;

        SimpleSocket(const SimpleSocket&) = delete; // not copyable
        SimpleSocket& operator=(const SimpleSocket&) = delete; // not copyable

        SimpleSocket(SimpleSocket&& other) = default; // movable
        SimpleSocket& operator=(SimpleSocket&& other) = default; // movable

        // Factory patterns
        static std::unique_ptr<SimpleSocket> createConnectedSocket(const std::string& hostname, const std::string& port, addrinfo* out_addr_info);
        static std::unique_ptr<SimpleSocket> createUdpListener(const std::string& port, int family);

        // High-level operations
        bool bind(const std::string& port);
        bool listen(int backlog = 10);

        // Medium-level operations
        bool connect(const addrinfo& addr_info);

        // Low-level operations
        bool connect(const void* sockaddr_ptr, int addr_len);
        bool bind(const void* sockaddr_ptr, int addr_len);

        // Address is filled if not null
        std::unique_ptr<SimpleSocket> accept(std::string* address); // For server sockets

        // TCP-specific
        int send(const char* data, size_t size);
        int recv(char* buffer, size_t size);

        // UDP-specific
        int sendto(const std::string& message, sockaddr* dest, int destlen);
        int recvfrom(void* buffer, size_t size, int flags, sockaddr* addr, int* addrlen);

        bool is_valid() const noexcept;
        void close();
        std::string get_error() const;

        // TODO: High-level UDP methods
        //   - int sendTo(const std::string& message, const std::string& host, const std::string& port);
        //   - int recvFrom(std::string& message, std::string& from_host, std::string& from_port);
        // TODO: Add port extraction helper for UDP recvFrom
        // TODO: Convenience methods for complete transfers
        //   - bool sendAll(const char* data, size_t size);
        //   - bool recvExact(char* buffer, size_t expected_size);
        // TODO: Socket options
        //   - bool setReuseAddress(bool enable);
        //   - bool setTimeout(int milliseconds);


    private:
        class Impl; // Platform-specific implemetation
        std::unique_ptr<Impl> m_impl;
        // Used for accept, move to avoid double closing of the socket!
        explicit SimpleSocket(Impl&& existing_socket);
    };

    class SimpleAddrinfo {
    public:
        // Constructor ensures platform is initialized
        explicit SimpleAddrinfo(const std::string& hostname, const std::string& port,
                                int family, int socktype, int flags);
        ~SimpleAddrinfo() = default; // unique_pointer handles cleanup!

        SimpleAddrinfo(const SimpleAddrinfo&) = delete; // Not copyable, not movable
        SimpleAddrinfo& operator=(const SimpleAddrinfo&) = delete;
        SimpleAddrinfo(SimpleAddrinfo&&) = default;
        SimpleAddrinfo& operator=(SimpleAddrinfo&&) = default;

        static const std::string getIPVersion(const addrinfo& in) noexcept;
        static const std::string getIP(const addrinfo& in) noexcept;
        static const std::string getIP(const sockaddr_storage& in) noexcept;
        static const std::string getIP(const sockaddr* in) noexcept;

        // Wrapper that works with std algorithmos (for range for)
        struct iterator {
            addrinfo* ptr = nullptr; // Raw pointer - does not own the data!
            addrinfo& operator*() const;
            addrinfo* operator->() const;
            iterator& operator++();
            bool operator!=(const iterator& other) const;
            bool operator==(const iterator& other) const;
        };

        // Range inference
        iterator begin() const noexcept;
        iterator end() const noexcept;
        const addrinfo* get() const noexcept;

        private:
        std::unique_ptr<addrinfo, void(*)(addrinfo*)> m_aiptr;
    };

}
