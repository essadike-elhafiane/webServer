/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 21:28:36 by eelhafia          #+#    #+#             */
/*   Updated: 2023/10/17 04:29:33 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/webserv.hpp"
#include "server/server.hpp"
#include "Client/Client.hpp"
#include "request/request.hpp"
#include <fcntl.h>
#include <poll.h>
#include <map>

void sigintHandler(int signal) {
    std::cout << "Received SIGINT signal. Cleaning up and exiting..." << std::endl;
    exit(signal);
}

#define MAX_CLIENTS 3
void v()
{
    system("leaks webserv");
}
int main()
{
    atexit(v);
    server a("server1");
    a.runServer(5, 8000);
    fcntl(a.getServerSocket(), F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    int serverSocket, clientSocket, i;
    serverSocket = a.getServerSocket();
    struct pollfd fds[MAX_CLIENTS + 1];
    memset(fds, 0, sizeof(fds));
    fds[0].fd = serverSocket;
    fds[0].events = POLLIN;
    // fds[0].events = 0;
    std::map<int, Client> mClients;
    // for (i = 1; i <= MAX_CLIENTS; i++) {
    //     fds[i].fd = -1;
    // }
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
            Client a;
            a.setClientSocket(clientSocket);
            if (fcntl(clientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)
            {
                perror("fcnl failed");
            }
            mClients[clientSocket] = a;
            // Add new client socket to the poll descriptor list
            for (i = 1; i <= MAX_CLIENTS; i++) 
            {
                if (fds[i].fd == 0) {
                    fds[i].fd = clientSocket;
                    fds[i].events = POLLIN;
                    break;
                }
            }
        }

        // Check for activity on client sockets
        for (i = 1; i <= MAX_CLIENTS; i++) {
            if (fds[i].fd != 0 && (fds[i].revents & POLLIN)){
                clientSocket = fds[i].fd;
                request request;
                request.receiveRequest(mClients[clientSocket]);
                // fds[i].fd = clientSocket;
                fds[i].fd = mClients[clientSocket].getClientSocket();
                // std::cout << clientSocket << "||" << mClients[clientSocket].getClientSocket() << std::endl;
                // close(clientSocket);
                // fds[i].events = POLLOUT;
            }
        }
        
    }

    close(a.getServerSocket());
    return 0;
}
