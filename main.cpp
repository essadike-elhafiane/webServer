/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 21:28:36 by eelhafia          #+#    #+#             */
/*   Updated: 2023/10/04 23:15:47 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/webserv.hpp"
#include "server/server.hpp"
#include "socketClient/socketClient.hpp"
#include "request/request.hpp"

int main()
{
    server a("server1");
    a.runServer(1024, 3000);
    fd_set current_socket, ready_socket;
    FD_ZERO(&current_socket);
    FD_SET(a.getServerSocket(), &current_socket);
    std::set<int> clients;
    clients.insert(a.getServerSocket());
    socketClient client;
    while (true) 
    {
        ready_socket = current_socket; // Create a new fd_set for each iteration
        if (select(FD_SETSIZE, &ready_socket, NULL, NULL, NULL) < 0) {
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