/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 21:28:36 by eelhafia          #+#    #+#             */
/*   Updated: 2023/10/22 20:02:29 by eelhafia         ###   ########.fr       */
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

#define MAX_CLIENTS 1000

// void v()
// {
//     system("leaks webserv");
// }
int main(int ac , char **av)
{
    // atexit(v);
    std::vector<HTTP_SERVER> configData;
    configFile(ac, av ,configData);
    std::vector<server> servers;
    struct pollfd fds[MAX_CLIENTS + 1];
    size_t numberServer = 0;
    memset(fds, 0, sizeof(fds));
    for (size_t i = 0; i < configData.size(); i++)
    {
        for (size_t j = 0; j < configData[i].port.size(); j++)
        {
            server a(configData[i].server_name);
            if (a.runServer(MAX_CLIENTS, configData[i].port[j]))
                continue;
            fcntl(a.getServerSocket(), F_SETFL, O_NONBLOCK, FD_CLOEXEC);
            fds[numberServer].fd = a.getServerSocket();
            fds[numberServer].events = POLLIN;
            servers.push_back(a);
            numberServer++;
        }
    }
    int clientSocket, i;
    std::map<int, Client> mClients;
    std::cout<< "number servers: " << numberServer << std::endl;
    while (true) 
    {
        int activity = poll(fds, MAX_CLIENTS + 1, 1000);
        if (activity < 0) {
            perror("Poll error");
            exit(EXIT_FAILURE);
        }

        // Check for activity on the server socket
        for (size_t i = 0; i < numberServer; i++)
        {
            if (fds[i].fd != 0 && fds[i].revents & POLLIN) 
            {
                std::cout << servers[i].getName() << "|" << servers[i].port << std::endl;
                if ((clientSocket = accept(servers[i].getServerSocket(), NULL, NULL)) < 0) {
                    perror("Accept error");
                    exit(EXIT_FAILURE);
                }
                Client a;
                a.setClientSocket(clientSocket);
                a.nameServer = servers[i].getName();
                a.configData = configData[i];
                if (fcntl(clientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)
                {
                    perror("fcnl failed");
                }
                mClients.erase(clientSocket);
                mClients[clientSocket] = a;
                // Add new client socket to the poll descriptor list
                for (i = numberServer; i <= MAX_CLIENTS; i++) 
                {
                    if (fds[i].fd == 0) {
                        fds[i].fd = clientSocket;
                        fds[i].events = POLLIN;
                        fds[i].revents = 0;
                        break;
                    }
                }
            }
        }
        
        // Check for activity on client sockets
        for (i = numberServer; i <= MAX_CLIENTS; i++) {
            if (fds[i].fd != 0 && (fds[i].revents & POLLIN)){
                clientSocket = fds[i].fd;
                request request;
                request.receiveRequest(mClients[clientSocket]);
                if (mClients[clientSocket].error)
                {
                    fds[i].events = POLLOUT;
                    fds[i].revents = 0;
                    continue;
                }
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
                // if (dataClient.getUrl() == "/")
                //     dataClient.setUrl("/html/file.html");
                // if (dataClient.getUrl() == "/Users/eelhafia/Desktop/y.mp4")
                    u = dataClient.getUrl();
                // else
                //     u = "/Users/eelhafia/Desktop/webServer" + dataClient.getUrl();
                // std::cout << dataClient.getUrl() << std::endl;
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
    for (size_t i = 0; i < 5; i++)
        close(servers[i].getServerSocket());
    return 0;
}
