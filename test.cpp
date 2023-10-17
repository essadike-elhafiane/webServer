#include <iostream>
#include <unistd.h>

#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

const int MAX_CLIENTS = 10;
const int BUFFER_SIZE = 1024;

int main() {
    int serverSocket, newSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    char buffer[BUFFER_SIZE];

    // Create server socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Set server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);  // Set the desired port
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind server socket to address
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        std::cerr << "Failed to listen for connections" << std::endl;
        return 1;
    }

    std::vector<int> clientSockets;
    std::vector<struct pollfd> fds(1); // 0 index for server socket
    fds[0].fd = serverSocket;
    fds[0].events = POLLIN | POLLOUT;

    std::cout << "Server is running. Waiting for connections..." << std::endl;

    while (true) {
        // Use poll to check for readable and writable sockets
        int pollResult = poll(fds.data(), fds.size(), -1);

        if (pollResult == -1) {
            std::cerr << "Error occurred in poll" << std::endl;
            return 1;
        }

        // Check server socket for new connection
        if (fds[0].revents & POLLIN) {
            // New client connection
            newSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLength);

            if (newSocket == -1) {
                std::cerr << "Failed to accept client connection" << std::endl;
                continue;
            }
            struct pollfd f;
            f.events = POLLIN | POLLOUT;
            f.fd = newSocket;
            fds.push_back(f);
            clientSockets.push_back(newSocket);

            std::cout << "New client connected: " << inet_ntoa(clientAddress.sin_addr) << std::endl;
        }

        // Check client sockets for read and write operations
        for (size_t i = 1; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                // Existing client sent data
                int clientSocket = fds[i].fd;
                memset(buffer, 0, sizeof(buffer));
                ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

                if (bytesRead == -1) {
                    std::cerr << "Failed to receive data from client" << std::endl;
                    continue;
                }

                if (bytesRead == 0) {
                    // Client disconnected
                    close(clientSocket);
                    fds.erase(fds.begin() + i);
                    clientSockets.erase(clientSockets.begin() + (i - 1));
                    std::cout << "Client disconnected" << std::endl;
                    --i;
                } else {
                    // Process received data
                    // Here, you can implement your own logic to handle the incoming data
                    std::cout << "Received data from client: " << buffer << std::endl;
                }
            }

            if (fds[i].revents & POLLOUT) {
                // Existing client is ready for write
                int clientSocket = fds[i].fd;
                std::string message = "Hello from server!";
                ssize_t bytesSent = send(clientSocket, message.c_str(), message.size(), 0);

                if (bytesSent == -1) {
                    std::cerr << "Failed to send data to client" << std::endl;
                    continue;
                }

                std::cout << "Sent data to client: " << message << std::endl;
            }
        }
    }

    // Close server socket
    close(serverSocket);

    return 0;
}
// int main() {
//     int loadingBarWidth = 40;
//     // std::string loadingAnimation = "|/-\\";
    
//     // for (int i = 0; i < 100; ++i) {
//     //     std::cout << "Loading: [";
        
//     //     int progress = (i * loadingBarWidth) / 100;
//     //     int animationIndex = i % loadingAnimation.size();
        
//     //     for (int j = 0; j < loadingBarWidth; ++j) {
//     //         if (j < progress) {
//     //             std::cout << "=";
//     //         } else if (j == progress) {
//     //             std::cout << loadingAnimation[animationIndex];
//     //         } else {
//     //             std::cout << " ";
//     //         }
//     //     }
        
//     //     std::cout << "] " << i << "%";
//     //     std::cout.flush();
        
//     //     usleep(100000); // Sleep for 100 milliseconds
//     //     std::cout << "\r"; // Move the cursor to the beginning of the line
//     // }
//     for (size_t i = 0; i < 1; i++)
//     {
//         for (size_t i = 0; i < 10; i++)
//         {
//             std::cout << "h" ;
//         }
        
//         std::cout << "\r";
//         std::cout << "aaaaaaaaaaahhhhhhhhhhaaaaaaaaaaa";
//         std::cout << "\r";
//         std::cout << "ss";
//     }
    
//     std::cout << std::endl;
    
//     return 0;
// }
