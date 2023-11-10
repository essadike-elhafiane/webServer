/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edraidry <edraidry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 21:28:36 by edraidry          #+#    #+#             */
/*   Updated: 2023/11/05 15:46:26 by edraidry         ###   ########.fr       */
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


#define MAX_CLIENTS 100

int main(int ac , char **av)
{
    std::vector<HTTP_SERVER> configData;
    configFile(ac, av ,configData);
    std::vector<server> servers;
    struct pollfd fds[MAX_CLIENTS + 1];
    size_t numberServer = 0;
    memset(fds, 0, sizeof(fds));
    signal(SIGPIPE, SIG_IGN);
    for (size_t i = 0; i < configData.size(); i++)
    {
        for (size_t j = 0; j < configData[i].port.size(); j++)
        {
            server a(configData[i].server_name, configData[i].hostname);
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
        int activity = poll(fds, MAX_CLIENTS + 1, 10000);
        if (activity < 0) {
            perror("Poll error");
        }
        if (activity == 0)
        {
            for (size_t i = numberServer; i < MAX_CLIENTS; i++)
            {
                if (fds[i].fd != 0)
                {
                    mClients.erase(fds[i].fd);
                    std::cout<< "close with timeout : " << fds[i].fd << std::endl;
                    close(fds[i].fd);
                    fds[i].fd = 0;
                    fds[i].events = 0;
                    fds[i].revents = 0;
                }
            }
        }

        // Check for activity on the server socket
        for (size_t i = 0; i < numberServer; i++)
        {
            if (fds[i].fd != 0 && fds[i].revents & POLLIN) 
            {
                for (size_t i = 0; i < 100000; i++)
                {}
                if ((clientSocket = accept(servers[i].getServerSocket(), NULL, NULL)) < 0) {
                    perror("Accept error");
                    // exit(EXIT_FAILURE);
                    continue;
                }
                
                Client a;
                a.setClientSocket(clientSocket);
                a.nameServer = servers[i].getName();
                a.dataServers = configData;
                std::cout<< servers[i].getName() << "|" << servers[i].port << " accept client : " << a.getClientSocket() << std::endl;
                size_t posServerData = 0;
                while (posServerData < configData.size())
                {
                    if (a.nameServer == configData[posServerData].server_name)
                        break;
                    posServerData++;
                }
                        
                a.configData = configData[posServerData];
                if (fcntl(clientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)
                {
                    perror("fcnl failed");
                }
                mClients.erase(clientSocket);
                mClients[clientSocket] = a;
                for (i = numberServer; i < MAX_CLIENTS; i++) 
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
        

        for (i = numberServer; i < MAX_CLIENTS; i++) {
            if (fds[i].fd != 0 && (fds[i].revents & POLLIN)){
                clientSocket = fds[i].fd;
                request request;
                request.receiveRequest(mClients[clientSocket]);
                if (mClients[clientSocket].error)
                {
                    fds[i].events = POLLOUT;
                    fds[i].revents = 0;
                }
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
                fds[i].events = POLLOUT | POLLIN;
                fds[i].revents = 0;
            }
            else if (fds[i].fd != 0 && (fds[i].revents & POLLOUT))
            {
                response resp;
                std::string u;
                Client &dataClient = mClients[fds[i].fd];
                if (!dataClient.error && dataClient.getCgi() != "" && (dataClient.getUrl().find(".php") != std::string::npos || dataClient.getUrl().find(".py") != std::string::npos))
                {
                    resp.sendStringResponse(dataClient);
                    dataClient.setTypeRequset("");
                    dataClient.setHeaderStatus(false);
                    dataClient.setUrl("");
                    dataClient.resetRestRequest();
                    fds[i].events = POLLIN;
                    fds[i].revents = 0;
                    if (!dataClient.error)
                        continue;
                }
                else if (!resp.sendResponse(dataClient))
                {
                    dataClient.setTypeRequset("");
                    dataClient.setHeaderStatus(false);
                    dataClient.setUrl("");
                    dataClient.resetRestRequest();
                    fds[i].events = POLLIN;
                    fds[i].revents = 0;
                }
                else
                {
                    fds[i].events =  POLLIN | POLLOUT;
                    fds[i].revents = 0;
                }
                if (dataClient.error || dataClient.connection == "close")
                {
                    mClients.erase(fds[i].fd);
                    std::cout<< "close with Error :" << fds[i].fd << std::endl;
                    close(fds[i].fd);
                    fds[i].fd = 0;
                    fds[i].events = 0;
                    fds[i].revents = 0;
                    continue;
                }
            }
            
        }
        if (fds[i].fd != 0 && (fds[i].revents & (POLLERR | POLLHUP)))
        {
            mClients.erase(fds[i].fd);
            std::cout<< "Close Client with Error : " << fds[i].fd << std::endl;
            close(fds[i].fd);
            fds[i].fd = 0;
            fds[i].events = 0;
            fds[i].revents = 0;
        }
    }
    for (size_t i = 0; i < 5; i++)
        close(servers[i].getServerSocket());
    return 0;
}

// just get and post work in cgi
// to_string ;
