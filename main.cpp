/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 21:28:36 by eelhafia          #+#    #+#             */
/*   Updated: 2023/10/05 00:51:14 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/webserv.hpp"
#include "server/server.hpp"
#include "socketClient/socketClient.hpp"
#include "request/request.hpp"
#include <fcntl.h>

void sigintHandler(int signal) {
    std::cout << "Received SIGINT signal. Cleaning up and exiting..." << std::endl;

    // Perform any necessary cleanup tasks here

    // Terminate the program
    exit(signal);
}

int main()
{
    signal(SIGINT, sigintHandler);
    server a("server1");
    a.runServer(10, 3000);
    fd_set current_socket, ready_socket;
    fcntl(a.getServerSocket(), F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    FD_ZERO(&current_socket);
    FD_SET(a.getServerSocket(), &current_socket);
    std::set<int> clients;
    clients.insert(a.getServerSocket());
    socketClient client;
    timeval timeout;
    timeout.tv_sec = 5;  // Timeout duration in seconds
    timeout.tv_usec = 0;
    while (true) 
    {
        ready_socket = current_socket; // Create a new fd_set for each iteration
        if (select(FD_SETSIZE, &ready_socket, NULL, NULL, &timeout) < 0) {
            std::cout << "ERROR select\n";
            exit(1);
        }
        for (std::set<int>::iterator itr = clients.begin(); itr != clients.end(); ) {
            if (FD_ISSET(*itr, &ready_socket)) {
                std::cout << *itr << std::endl;
                if (*itr == a.getServerSocket()) {
                    client.setUpAndAccept(a);
                    clients.insert(client.getClientSocket());
                    FD_SET(client.getClientSocket(), &current_socket);
                } else {
                    request request;
                    request.receiveRequest(client);
                    int clientSocket = client.getClientSocket();
                    FD_CLR(clientSocket, &current_socket);
                    close(clientSocket);
                    itr = clients.erase(itr);
                    continue;
                }
            }
            ++itr; // Increment iterator
        }
    }
    // while (true) {
    //     ready_socket = current_socket;
    //     if (select(FD_SETSIZE, &ready_socket, NULL, NULL, NULL) < 0)
    //     {
    //         std::cout << "ERRROR select\n";
    //         exit(1);
    //     }
    //     for (std::set<int>::iterator itr = clients.begin(); itr != clients.end() ;itr++)
    //     {
    //         std::cout << *itr << std::endl;
    //         if (FD_ISSET(*itr, &ready_socket))
    //         {
    //             if (*itr == a.getServerSocket())
    //             {
    //                 client.setUpAndAccept(a); 
    //                 clients.insert(client.getClientSocket());
    //                 FD_SET(client.getClientSocket(), &current_socket);
    //             }
    //             else
    //             {
    //                 request request;
    //                 request.receiveRequest(client);
    //                 FD_CLR(*itr, &current_socket);
    //                 close(client.getClientSocket());
    //                 // clients.erase(*itr);
    //             }
    //         }
    //     }
    // }

    close(a.getServerSocket());
    return 0;
}