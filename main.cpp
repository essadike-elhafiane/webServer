/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 21:28:36 by eelhafia          #+#    #+#             */
/*   Updated: 2023/10/01 05:32:03 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/webserv.hpp"

int main() {
    int serverSocket, clientSocket;
    socklen_t clientLength;
    // char buffer[256];
    struct sockaddr_in serverAddress, clientAddress;

    // Create the server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error opening socket." << std::endl;
        return 1;
    }

    // Set up the server address structure
    std::memset((char*)&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Bind to any available address
    serverAddress.sin_port = htons(8080); // Use a port that is allowed by your cloud provider

    // Bind the server socket to the specified address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error binding socket." << std::endl;
        return 1;
    }

    // Listen for incoming connections
    listen(serverSocket, 10);
    std::cout << "Server listening on 8080..." << std::endl;

    std::string requests;
    // Accept client connections and respond
    char b[30000];
    while (true) {
        clientLength = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLength);
        if (clientSocket < 0) {
            std::cerr << "Error accepting client connection." << std::endl;
            return 1;
        }
        int len = recv(clientSocket, b, 30000 - 1, 0);
        b[len] = '\0';
        requests += b;
        // std::string message = "Hello from server!";
        // std::strcpy(buffer, message.c_str());
        // write(clientSocket, buffer, message.length());
        std::cout << requests << std::endl;
        std::string rOK = "HTTP/1.1 200 OK\r\nContent-Length: ";
        std::ifstream r("html/file.html");
        if (!r.is_open())
            exit(1);
            
        std::string response;
        std::string line; 
        while (!std::getline(r, line).fail())
            response = response + line + '\n';
        
        rOK = rOK + std::to_string(response.length()) + "\r\n\r\n" + response;
        std::cout << std::endl << std::endl;
        // std::cout << rOK << std::endl;
        if (send(clientSocket, rOK.c_str(), rOK.length(), 0) < 0)
        {
            std::cerr << "Failed to send response." << std::endl;
            close(clientSocket);
            continue;
        }
        close(clientSocket);
        // exit(1);
    }

    close(serverSocket);
    return 0;
}