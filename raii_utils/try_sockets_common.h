#pragma once
#include <string>

// Common interface for both platforms
class SocketClient {
public:
    SocketClient();
    ~SocketClient();

    int connect_to_host(const std::string& host, const std::string& port);
    int send_all(int fd, const char* data, size_t len);
    int receive_data(int fd, char* buffer, size_t buffer_size);
    void close_socket(int fd);

private:
    bool initialized;
};

// Free functions for compatibility with existing code
int connect_to_host(const std::string& host, const std::string& port);
int send_all(int fd, const char* data, size_t len);