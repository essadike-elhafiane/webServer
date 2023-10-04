#include <iostream>
#include <cstring>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <arpa/inet.h> 

int main() {
    const int MAX_CLIENTS = 10;
    const int BUFFER_SIZE = 1024;
    const int PORT = 8080;

    int serverSocket, clientSockets[MAX_CLIENTS], maxSocket;
    fd_set readfds;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serverAddr;
    socklen_t addrSize = sizeof(serverAddr);

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        std::cerr << "Failed to set socket options." << std::endl;
        return 1;
    }

    // Bind socket to port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, addrSize) == -1) {
        std::cerr << "Failed to bind socket." << std::endl;
        return 1;
    }

    // Listen for connections
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Failed to listen on socket." << std::endl;
        return 1;
    }

    // Initialize client sockets
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        clientSockets[i] = 0;
    }

    // Main server loop
    while (true) {
        FD_ZERO(&readfds);
        FD_SET(serverSocket, &readfds);
        maxSocket = serverSocket;

        // Add active client sockets to the set
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            int clientSocket = clientSockets[i];
            if (clientSocket > 0) {
                FD_SET(clientSocket, &readfds);
                if (clientSocket > maxSocket) {
                    maxSocket = clientSocket;
                }
            }
        }

        // Wait for activity on any socket
        int activity = select(maxSocket + 1, &readfds, nullptr, nullptr, nullptr);
        if (activity == -1) {
            std::cerr << "Error in select." << std::endl;
            return 1;
        }

        // Check for new incoming connection
        if (FD_ISSET(serverSocket, &readfds)) {
            int newSocket = accept(serverSocket, (struct sockaddr*)&serverAddr, &addrSize);
            if (newSocket == -1) {
                std::cerr << "Failed to accept connection." << std::endl;
                return 1;
            }

            // Add new client socket to the array
            for (int i = 0; i < MAX_CLIENTS; ++i) {
                if (clientSockets[i] == 0) {
                    clientSockets[i] = newSocket;
                    break;
                }
            }
        }

        // Handle requests from active client sockets
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            int clientSocket = clientSockets[i];
            if (FD_ISSET(clientSocket, &readfds)) {
                int bytesRead = read(clientSocket, buffer, BUFFER_SIZE);
                if (bytesRead == 0) {
                    // Connection closed by client
                    close(clientSocket);
                    clientSockets[i] = 0;
                } else if (bytesRead == -1) {
                    std::cerr << "Error in reading from client socket." << std::endl;
                } else {
                    // Handle the received request
                    // Here you can parse the request, generate a response, and send it back to the client
                    // For simplicity, we'll just print the received data
                    buffer[bytesRead] = '\0';
                    std::cout << "Received request: " << buffer << std::endl;
                }
           }
        }
    }
    return 0;
}