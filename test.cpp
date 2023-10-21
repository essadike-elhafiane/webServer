#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main() {
    const char* hostname = "essadikuuue.u";
    const int port = 8004;

    struct addrinfo hints, *res;
    std::memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP socket

    int status = getaddrinfo(hostname, NULL, &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return 1;
    }

    // Iterate through the linked list of addrinfo structures
    struct addrinfo* addr;
    int sockfd = -1;
    for (addr = res; addr != NULL; addr = addr->ai_next) {
        sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (sockfd == -1) {
            std::cerr << "socket error" << std::endl;
            continue;
        }

        if (bind(sockfd, addr->ai_addr, addr->ai_addrlen) == -1) {
            std::cerr << "bind error" << std::endl;
            close(sockfd);
            sockfd = -1;
            continue;
        }

        break;  // Successfully bound socket
    }

    if (addr == NULL) {
        std::cerr << "Failed to bind socket to any address" << std::endl;
        freeaddrinfo(res);
        return 1;
    }

    freeaddrinfo(res);

    std::cout << "Socket bound successfully!" << std::endl;

    close(sockfd);

    return 0;
}
