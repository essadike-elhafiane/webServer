/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 21:28:36 by eelhafia          #+#    #+#             */
/*   Updated: 2023/10/20 01:27:28 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/webserv.hpp"
#include "server/server.hpp"
#include "Client/Client.hpp"
#include "request/request.hpp"
#include <fcntl.h>
#include <poll.h>
#include <map>
#include "config/parsing.hpp"

void sigintHandler(int signal) {
    std::cout << "Received SIGINT signal. Cleaning up and exiting..." << std::endl;
    exit(signal);
}

#define MAX_CLIENTS 30
void v()
{
    system("leaks webserv");
}
int main(int ac, char **av)
{
    atexit(v);
    std::vector<HTTP_SERVER>& configData = configFile(ac, av);
    
    server a(configData[0].server_name);
    a.runServer(MAX_CLIENTS, configData[0].port[0]);
    fcntl(a.getServerSocket(), F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    int serverSocket, clientSocket, i;
    serverSocket = a.getServerSocket();
    struct pollfd fds[MAX_CLIENTS + 1];
    memset(fds, 0, sizeof(fds));
    fds[0].fd = serverSocket;
    fds[0].events = POLLIN ;
    std::map<int, Client> mClients;
    while (true) 
    {
        int activity = poll(fds, MAX_CLIENTS + 1, 1000);
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
            mClients.erase(clientSocket);
            mClients[clientSocket] = a;
            // Add new client socket to the poll descriptor list
            for (i = 1; i <= MAX_CLIENTS; i++) 
            {
                if (fds[i].fd == 0) {
                    fds[i].fd = clientSocket;
                    fds[i].events = POLLIN;
                    fds[i].revents = 0;
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
                
                // close(clientSocket);
                // fds[i].events = POLLOUT;
                std::cout << clientSocket << "||" << mClients[clientSocket].getClientSocket() << std::endl;
                if (!mClients[clientSocket].getClientSocket())
                {
                    mClients.erase(fds[i].fd);
                    fds[i].fd = 0;
                    fds[i].events = 0;
                    fds[i].revents = 0;
                    continue;
                }
                if (mClients[clientSocket].getTypeRequset() == "POST" && mClients[clientSocket].getReadlen() < mClients[clientSocket].getContentLength())
                    continue;
                fds[i].events = POLLOUT;
                fds[i].revents = 0;
            }
            else if (fds[i].fd != 0 && (fds[i].revents & POLLOUT))
            {
                std::cout << "\n1\n";
                response resp;
                std::string u;
                Client &dataClient = mClients[fds[i].fd];
                if (dataClient.getUrl() == "/")
                    dataClient.setUrl("/html/file.html");
                if (dataClient.getUrl() == "/Users/eelhafia/Desktop/y.mp4")
                    u = dataClient.getUrl();
                else
                    u = "/Users/eelhafia/Desktop/webServer" + dataClient.getUrl();
                std::cout << dataClient.getUrl() << std::endl;
                std::string rOK = "HTTP/1.1 200 OK\r\nContent-Length: ";
                // if (dataClient.get)
                if (!resp.sendResponse(u, rOK, fds[i].fd, dataClient))
                {
                    dataClient.setTypeRequset("");
                    dataClient.setHeaderStatus(false);
                    dataClient.setUrl("");
                    dataClient.resetRestRequest();
                    // dataClient.resetData();
                    // dataClient.resetData();
                    // if (!fds[i].fd)
                    // {
                    //     std::cout << "booo\n";
                    //     continue;
                    // }
                    // if (datac)
                    fds[i].events = POLLIN;
                    fds[i].revents = 0;
                }
                
                // fds[i].events = POLLIN;
            }
            
        }
        // std::cout << "2\n";
    }

    close(a.getServerSocket());
    return 0;
}
