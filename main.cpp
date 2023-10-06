/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 21:28:36 by eelhafia          #+#    #+#             */
/*   Updated: 2023/10/06 15:16:57 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/webserv.hpp"
#include "server/server.hpp"
#include "socketClient/socketClient.hpp"
#include "request/request.hpp"
#include <fcntl.h>
#include <poll.h>
// #include <winsock2.h>

void sigintHandler(int signal) {
    std::cout << "Received SIGINT signal. Cleaning up and exiting..." << std::endl;

    // Perform any necessary cleanup tasks here

    // Terminate the program
    exit(signal);
}

#define MAX_CLIENTS 10

int main()
{
    server a("server1");
    a.runServer(5, 3000);
    fcntl(a.getServerSocket(), F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    int serverSocket, clientSocket, i;
    serverSocket = a.getServerSocket();
    struct pollfd fds[MAX_CLIENTS + 1];
    fds[0].fd = serverSocket;
    fds[0].events = POLLIN;

    for (i = 1; i <= MAX_CLIENTS; i++) {
        fds[i].fd = -1;
    }

    while (true) 
    {
        int activity = poll(fds, MAX_CLIENTS + 1, -1);
        if (activity < 0) {
            perror("Poll error");
            exit(EXIT_FAILURE);
        }

        // Check for activity on the server socket
        if (fds[0].revents & POLLIN) 
        {
            if ((clientSocket = accept(serverSocket, NULL, NULL)) < 0) {
                perror("Accept error");
                exit(EXIT_FAILURE);
            }

            // Add new client socket to the poll descriptor list
            for (i = 1; i <= MAX_CLIENTS; i++) 
            {
                if (fds[i].fd == -1) {
                    fds[i].fd = clientSocket;
                    fds[i].events = POLLIN;
                    break;
                }
            }
        }

        // Check for activity on client sockets
        for (i = 1; i <= MAX_CLIENTS; i++) {
            if (fds[i].fd != -1 && (fds[i].revents & POLLIN)){
                clientSocket = fds[i].fd;
                request request;
                request.receiveRequest(clientSocket);
            //     if (request.getUrl()== "/html/html/compte.html" || request.getUrl()== "/html/compte.html")
            //     {
            //         std::string rOK = "HTTP/1.1 200 OK\r\nContent-Length: ";
            //         std::ifstream r("html/compte.html");
            //         if (!r.is_open())
            //             exit(1);
            //         std::string response;
            //         std::string line; 
            //         while (!std::getline(r, line).fail())
            //             response = response + line + '\n';
                    
            //         rOK = rOK + std::to_string(response.length()) + "\r\n\r\n" + response;
            //         std::cout << std::endl << std::endl;
            //         std::cout << rOK << std::endl;
            //         if (send(clientSocket, rOK.c_str(), rOK.length(), 0) < 0)
            //         {
            //             std::cerr << "Failed to send response." << std::endl;
            //             close(clientSocket);
            //             continue;
            //         }
            //         // close(clientSocket);
            //         // std::cout << std::endl << "|" << requests.substr(4, 17) << "|" << std::endl << std::endl;
            //         continue;
            //     }
            //     if (request.getUrl() == "/html/app-coder.html" || request.getUrl() == "/html/html/app-coder.html")
            //     {
            //         std::string rOK = "HTTP/1.1 200 OK\r\nContent-Length: ";
            //         std::ifstream r("html/app-coder.html");
            //         if (!r.is_open())
            //             exit(1);
            //         std::string response;
            //         std::string line; 
            //         while (!std::getline(r, line).fail())
            //             response = response + line + '\n';
                    
            //         rOK = rOK + std::to_string(response.length()) + "\r\n\r\n" + response;
            //         std::cout << std::endl << std::endl;
            //         std::cout << rOK << std::endl;
            //         if (send(clientSocket, rOK.c_str(), rOK.length(), 0) < 0)
            //         {
            //             std::cerr << "Failed to send response." << std::endl;
            //             close(clientSocket);
            //             continue;
            //         }
                    
            //         // close(clientSocket);
            //         // std::cout << std::endl << "|" << requests.substr(4, 25) << "|" << std::endl << std::endl;
            //         continue;
            //     }
            //     if (request.getUrl() == "/" || request.getUrl() == "/html/file.html")
            //     {
            //         std::string rOK = "HTTP/1.1 200 OK\r\nContent-Length: ";
            //         std::ifstream r("html/file.html");
            //         if (!r.is_open())
            //             exit(1);
                        
            //         std::string response;
            //         std::string line; 
            //         while (!std::getline(r, line).fail())
            //             response = response + line + '\n';
                    
            //         rOK = rOK + std::to_string(response.length()) + "\r\n\r\n" + response;
            //         std::cout << std::endl << std::endl;
            //         // std::cout << rOK << std::endl;
            //         if (send(clientSocket, rOK.c_str(), rOK.length(), 0) < 0)
            //         {
            //             std::cerr << "Failed to send response." << std::endl;
            //             close(clientSocket);
            //             continue;
            //         }
            //     }
            //     std::cout << clientSocket << std::endl;
            }
        }
        
    }

    close(a.getServerSocket());
    return 0;
}