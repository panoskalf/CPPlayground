#include <iostream>
#include <thread>
#include <memory>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "SimpleSocket.h"

int main(void){

    const std::string port("3490");
    su::SimpleSocket serv(AF_INET, SOCK_STREAM, 0);

    if (!serv.bind(port)) {
        std::cerr << "Failed to bind to port " << port << ": " << serv.get_error() << std::endl;
        return 1;
    }

    if (!serv.listen()) {
        std::cerr << "Failed to listen on port " << port << ": " << serv.get_error() << std::endl;
        return 1;
    }

    std::cout << "Server listening on port " << port << std::endl;

    while (true) {
        struct sockaddr_storage their_addr{};
        socklen_t sin_size = sizeof(their_addr);

        std::string s;
        auto pclient = serv.accept(&s);

        if (pclient) {
            std::cout << "server: got connection from " << s << std::endl;

            // Handle client in detached thread with better error checking
            std::thread([client = std::move(pclient)]() {
                try {
                    const std::string message = "Hello, world!";
                    std::cout << "About to send message: '" << message << "'" << std::endl;

                    int bytes_sent = client->send(message.c_str(), message.size());

                    if (bytes_sent == -1) {
                        std::cerr << "Send failed: " << client->get_error() << std::endl;
                    } else if (bytes_sent == 0) {
                        std::cout << "Warning: send() returned 0 bytes" << std::endl;
                    } else {
                        std::cout << "Sent " << bytes_sent << " bytes successfully" << std::endl;
                    }

                    // Optional: Add a small delay to ensure send completes
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                } catch (const std::exception& e) {
                    std::cerr << "Exception in client thread: " << e.what() << std::endl;
                }
            }).detach();
        } else {
            std::cerr << "Accept failed: " << serv.get_error() << std::endl;
        }
    }

    return 0;
}