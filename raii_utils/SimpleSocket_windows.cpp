// SimpleSocket.cpp (Windows implementation)
#include <stdexcept>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "SimpleSocket.h"

// Platform-specific initialization singleton for WSA
class PlatformInit {
public:
    static PlatformInit& instance() {
        static PlatformInit instance; // Thread-safe in >= C++11
        return instance;
    }
private:
    PlatformInit() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
            throw std::runtime_error("WSAStartup failed: " + std::to_string(WSAGetLastError()));
        }
    }
    ~PlatformInit() noexcept {
        int result = WSACleanup();
        if (result == SOCKET_ERROR) { // TODO: duplicate?
            // Can't throw from destructor - just ignore or log
            [[maybe_unused]] int error = WSAGetLastError();
        }
    }
    PlatformInit(const PlatformInit&) = delete; // not copyable, not movable
    PlatformInit& operator=(const PlatformInit&) = delete; // not copyable, not movable
};


// Platform-specific implementation for socket
class su::SimpleSocket::Impl {
public:
    SOCKET socket;

    Impl(int family, int socktype, int protocol) : socket(INVALID_SOCKET) {
        socket = ::socket(family, socktype, protocol);
        if (socket == INVALID_SOCKET) {
            throw std::runtime_error("Socket creation failed " + std::to_string(WSAGetLastError()));
        }
    }

    Impl(SOCKET other) : socket(other) {};

    ~Impl() noexcept {
        if (socket != INVALID_SOCKET) {
            int result = closesocket(socket);
            if (result == SOCKET_ERROR) { // TODO: duplicate?
                // Can't throw from destructor - just ignore or log
                [[maybe_unused]] int error = WSAGetLastError();
            }
        }
    }

};

su::SimpleSocket::SimpleSocket(int family, int socktype, int protocol) {
    [[maybe_unused]] auto& init = PlatformInit::instance(); // Force init, avoid warnings
    m_impl = std::make_unique<Impl>(family, socktype, protocol);
}

// delegating constructor
su::SimpleSocket::SimpleSocket(const addrinfo& ai)
: SimpleSocket(ai.ai_family, ai.ai_socktype, ai.ai_protocol) {}

su::SimpleSocket::~SimpleSocket() = default;

// constructor for accept
su::SimpleSocket::SimpleSocket(Impl&& existing_socket) {
    if (existing_socket.socket == INVALID_SOCKET) {
        throw std::runtime_error("Invalid socket provided");
    }
    m_impl = std::make_unique<Impl>(existing_socket.socket);
    // Transfer ownership - prevent double close
    existing_socket.socket = INVALID_SOCKET;
}

// Factory patterns
std::unique_ptr<su::SimpleSocket> su::SimpleSocket::createConnectedSocket(const std::string& host, const std::string& port, addrinfo* out_addr_info) {
    SimpleAddrinfo addr_info(host, port, AF_UNSPEC, SOCK_STREAM, 0); // client connection

    for (const auto& addr : addr_info) {
        SimpleSocket client(addr);
        if (client.connect(addr.ai_addr, static_cast<int>(addr.ai_addrlen))) { // Call low level directly
            if (out_addr_info) {*out_addr_info = std::move(addr);} // Move to caller
            return {std::make_unique<SimpleSocket>(std::move(client))}; // success
        }
    }
    return nullptr; // failed
}

std::unique_ptr<su::SimpleSocket> su::SimpleSocket::createUdpListener(const std::string& port, int family){
    SimpleAddrinfo addr_info("", port, family, SOCK_DGRAM, AI_PASSIVE);
                                            //use UDP ^^^ use my IP ^^^
    for (const auto& addr : addr_info) {
        auto socket = std::make_unique<SimpleSocket>(addr);
        if (socket->bind(addr.ai_addr, addr.ai_addrlen)) {
            return socket; // success
        }
    }
    return nullptr; // failed
}

// High-level operations
bool su::SimpleSocket::bind(const std::string& port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET; // IPv4 or IPv6
    addr.sin_addr.s_addr = INADDR_ANY; // Bind to all interfaces
    try {
        // stoul can throw
        unsigned long port_num = stoul(port);
        // Validate port range (0-65535, but 0 means "any available port")
        if (port_num > 65535) throw std::runtime_error("invalid port");
        addr.sin_port = htons(static_cast<USHORT>(port_num));
        return bind(&addr, sizeof(addr)); // delegate call to lower level function

    } catch (std::exception e) {
        std::cerr << "SimpleSocket::bind failed " << e.what() << std::endl;
        return false;
    }
}

bool su::SimpleSocket::listen(int backlog) {
    if (! is_valid()) return false;
    return ::listen(m_impl->socket, backlog) != SOCKET_ERROR;
}

// Medium-level operations
bool su::SimpleSocket::connect(const addrinfo& addr_info) {
    return this->connect(addr_info.ai_addr, static_cast<int>(addr_info.ai_addrlen)); // delegate to low level function
}

// Low-level operations
bool su::SimpleSocket::connect(const void* sockaddr_ptr, int addr_len) {
    if (! is_valid()) return false;
    return (::connect(m_impl->socket, reinterpret_cast<const sockaddr*>(sockaddr_ptr), addr_len) != SOCKET_ERROR);
}

bool su::SimpleSocket::bind(const void* sockaddr_ptr, int addr_len) {
    if (! is_valid()) return false;
    return ::bind(m_impl->socket, reinterpret_cast<const sockaddr*>(sockaddr_ptr), addr_len) != SOCKET_ERROR;
}

std::unique_ptr<su::SimpleSocket> su::SimpleSocket::accept(std::string* address) {
    if (! is_valid()) return nullptr;
    struct sockaddr_storage their_addr{};
    int size = sizeof(their_addr);

    SOCKET client = ::accept(m_impl->socket, reinterpret_cast<sockaddr*>(&their_addr), &size);

    if (client == INVALID_SOCKET) return nullptr;

    if(address) { // fill the address
        *address = su::SimpleAddrinfo::getIP(reinterpret_cast<sockaddr*>(&their_addr));
    }

    try {
        Impl client_impl(client);
        // Important: MOVE, otherwise a copy will be made and when
        // the original object is destroyed the socket is closed!
        return std::unique_ptr<SimpleSocket>(new SimpleSocket(std::move(client_impl)));
    } catch (const std::exception& e) {
        closesocket(client); // don't forget to close
        return nullptr;
    }
}

int su::SimpleSocket::send(const char* data, size_t size) {
    // Ensure size fits in int for Windows send()
    if (!is_valid() || size > INT_MAX) { return -1; }
    int result = ::send(m_impl->socket, data, static_cast<int>(size), 0);
    return result;
}

int su::SimpleSocket::recv(char* buffer, size_t size) {
    if (!is_valid() || size > INT_MAX) { return -1; }
    int result = ::recv(m_impl->socket, buffer, size, 0);
    return result;
}

// UDP-specific
int su::SimpleSocket::sendto(const std::string& message, sockaddr* dest, int destlen) {
    if (! is_valid()) return -1;
    return ::sendto(m_impl->socket, message.c_str(), message.size(), 0, dest, destlen);
}

int su::SimpleSocket::recvfrom(void* buffer, size_t size, int flags, sockaddr* addr, int* addrlen) {
    if (! is_valid()) return -1;
    return ::recvfrom(m_impl->socket, reinterpret_cast<char*>(buffer), size, flags, addr, addrlen);
}

bool su::SimpleSocket::is_valid() const noexcept {
    return m_impl && m_impl->socket != INVALID_SOCKET;
}

void su::SimpleSocket::close() {
    if (m_impl && m_impl->socket != INVALID_SOCKET) {
        closesocket(m_impl->socket);
        m_impl->socket = INVALID_SOCKET;
    }
}

std::string su::SimpleSocket::get_error() const {
    return "Error code: " + std::to_string(WSAGetLastError());
}


// SimpleAddrinfo
su::SimpleAddrinfo::SimpleAddrinfo(const std::string& hostname, const std::string& port,
                                int family, int socktype, int flags)
: m_aiptr(nullptr, [](addrinfo* ptr) { freeaddrinfo(ptr); })
{
    [[maybe_unused]] auto& init = PlatformInit::instance(); // Force init, avoid warnings

    struct addrinfo hints{}; // init hints to zero
    hints.ai_family = family;
    hints.ai_socktype = socktype;
    hints.ai_flags = flags;

    addrinfo* temp_result = nullptr;
    const char* host_ptr = hostname.empty() ? nullptr : hostname.c_str();
    const char* port_ptr = port.empty() ? nullptr : port.c_str();

    int result = getaddrinfo(host_ptr, port_ptr, &hints, &temp_result);
    if (result != 0) {
        throw std::runtime_error("getaddrinfo failed (code " + std::to_string(result) + ")");
    }

    m_aiptr.reset(temp_result); // transfer ownership to unique_ptr
}

const std::string su::SimpleAddrinfo::getIPVersion(const addrinfo& in) noexcept {
    if (in.ai_family == AF_INET) {
        return "IPv4";
    } else if (in.ai_family == AF_INET6) {
        return "IPv6";
    } else {
        return "Unknown address family";
    }
}

const std::string su::SimpleAddrinfo::getIP(const addrinfo& in) noexcept {
    return getIP(in.ai_addr);
}

const std::string su::SimpleAddrinfo::getIP(const sockaddr_storage& in) noexcept {
    return getIP(reinterpret_cast<const sockaddr*>(&in));
}

const std::string su::SimpleAddrinfo::getIP(const sockaddr* in) noexcept {
    const void* addr_ptr = nullptr;

    if (in->sa_family == AF_INET) { // IPv4
        auto* ipv4 = reinterpret_cast<const struct sockaddr_in *>(in);
        addr_ptr = &(ipv4->sin_addr);
    } else if (in->sa_family == AF_INET6) { // IPv6 (should be 23 on Windows)
        auto* ipv6 = reinterpret_cast<const struct sockaddr_in6 *>(in);
        addr_ptr = &(ipv6->sin6_addr);
    } else {
        return "Unknown address family";
    }
    // convert the IP to a string and print it:
    char ipstr[INET6_ADDRSTRLEN];
    if (inet_ntop(in->sa_family, addr_ptr, ipstr, sizeof(ipstr))) {
        return {ipstr};
    }

    return "Invalid address"; // ntop failed
}

su::SimpleAddrinfo::iterator su::SimpleAddrinfo::begin() const noexcept {
    return {m_aiptr.get()};
}

su::SimpleAddrinfo::iterator su::SimpleAddrinfo::end() const noexcept {
    return {nullptr};
}

const addrinfo* su::SimpleAddrinfo::get() const noexcept {
    return m_aiptr.get();
}

// SimpleAddrinfo iterator definitions
addrinfo& su::SimpleAddrinfo::iterator::operator*() const {
    return *ptr;
}

addrinfo* su::SimpleAddrinfo::iterator::operator->() const {
    return ptr;
}

su::SimpleAddrinfo::iterator& su::SimpleAddrinfo::iterator::operator++() {
    ptr = ptr ? ptr->ai_next : nullptr;
    return *this;
}

bool su::SimpleAddrinfo::iterator::operator!=(const iterator& other) const {
    return ptr != other.ptr;
}

bool su::SimpleAddrinfo::iterator::operator==(const iterator& other) const {
     return ptr == other.ptr;
}
