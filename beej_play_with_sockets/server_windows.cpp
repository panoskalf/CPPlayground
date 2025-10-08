#include <iostream>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

class SimpleServer {
public:
    SimpleServer() {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2,2), &wsaData);
    }

    ~SimpleServer() { WSACleanup(); }

    void start(int port) {
        SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        bind(listen_sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
        listen(listen_sock, 10);

        std::cout << "Server listening on port " << port << std::endl;

        while (true) {
            struct sockaddr_storage their_addr{};
            socklen_t sin_size = sizeof(their_addr);
            SOCKET client = accept(listen_sock, (struct sockaddr *)&their_addr, &sin_size);

            char s[INET6_ADDRSTRLEN];
            inet_ntop(their_addr.ss_family,
                get_in_addr((struct sockaddr *)&their_addr),
                s, sizeof(s));
            std::cout << "server: got connection from " << s << std::endl;
            if (client != INVALID_SOCKET) {
                // Detach thread
                std::thread([client]() {
                    send(client, "Hello, world!", 13, 0);
                    closesocket(client);
                }).detach();
            }
        }
    }
};

int main(void){
    SimpleServer serv{};
    serv.start(3490);

    return 0;
}